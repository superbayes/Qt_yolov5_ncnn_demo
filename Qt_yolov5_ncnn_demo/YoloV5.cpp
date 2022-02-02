//
// Created by 邓昊晴 on 14/6/2020.
//

#include "YoloV5.h"

bool YoloV5::hasGPU = true;
YoloV5 *YoloV5::detector = nullptr;

YoloV5::YoloV5(const char *param, const char *bin, bool useGPU) {
    Net = new ncnn::Net();
    // opt 需要在加载前设置
    hasGPU = ncnn::get_gpu_count() > 0;
    Net->opt.use_vulkan_compute = hasGPU && useGPU;  // gpu
    Net->opt.use_int8_arithmetic = true;  // fp16运算加速
    Net->load_param(param);
    Net->load_model(bin);
}

YoloV5::~YoloV5() {
    delete Net;
}

bool YoloV5::detect(cv::Mat& image, float threshold, float nms_threshold) 
{
    cv::Mat dst;
    this->resize_keepRatio(image, dst, this->scale_input,/* wPad,hPad,*/ this->input_size);

    ncnn::Mat in_net = ncnn::Mat::from_pixels(dst.data, ncnn::Mat::PIXEL_BGR, this->input_size, this->input_size);
    
    float norm[3] = {1 / 255.f, 1 / 255.f, 1 / 255.f};
    float mean[3] = {0, 0, 0};
    in_net.substract_mean_normalize(mean, norm);
    auto ex = Net->create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);
    hasGPU = ncnn::get_gpu_count() > 0;
    ex.set_vulkan_compute(false/*hasGPU*/);
    ex.input("images", in_net);

    for (const auto &layer: layers) {
        ncnn::Mat blob;
        ex.extract(layer.name.c_str(), blob);

        //int tmp_w = (640 / layer.stride) * (640 / layer.stride);
        //ncnn::Mat blob2 = blob.reshape(9, tmp_w, 3);
        //ncnn::Mat in_pack3;
        //ncnn::convert_packing(blob, in_pack3, 3);
        //cv::Mat a(blob.h, blob.w, CV_32FC3);
        //memcpy((uchar*)a.data, in_pack3.data, blob.w * blob.h * 3 * sizeof(float));

        auto boxes = decode_infer(blob, layer.stride, {(int)input_size, (int)input_size}, input_size,
                                  num_class, layer.anchors, threshold);
        this->detect_result.insert(this->detect_result.begin(), boxes.begin(), boxes.end());
    }
    nms(this->detect_result, nms_threshold);
    return true;
}

bool YoloV5::draw(cv::Mat& image)
{
    for (BoxInfo& box : this->detect_result)
    {
        box.x1 = (box.x1 - 0) / this->scale_input;
        box.x2 = (box.x2 - 0) / this->scale_input;
        box.y1 = (box.y1 - 0) / this->scale_input;
        box.y2 = (box.y2 - 0) / this->scale_input;
        cv::Rect rec(cv::Point(box.x1, box.y1), cv::Point(box.x2, box.y2));
        cv::rectangle(image, rec, cv::Scalar(0, 0, 255, 0), 2);
        char textName[1024];//需要预先分配缓冲区 labels
        sprintf_s(textName, "%s;%.3f;", this->labels[box.label], box.score);
        cv::putText(
            image, textName, cv::Point(rec.x, rec.y+10),
            cv::FONT_HERSHEY_PLAIN, 1,cv::Scalar(0, 0, 255)
        );
        //printf("rect x=%d;y=%d,w=%d,h=%d", rec.x, rec.y, rec.width, rec.height);
    }
    this->detect_result.clear();
    return true;
}

inline float fast_exp(float x) {
    union {
        uint32_t i;
        float f;
    } v{};
    v.i = (1 << 23) * (1.4426950409 * x + 126.93490512f);
    return v.f;
}

inline float sigmoid(float x) {
    return 1.0f / (1.0f + fast_exp(-x));
}

std::vector<BoxInfo>
YoloV5::decode_infer(ncnn::Mat &data, int stride, const yolocv::YoloSize &frame_size, int net_size, int num_classes,
                     const std::vector<yolocv::YoloSize> &anchors, float threshold) {
    std::vector<BoxInfo> result;
    int grid_size = int(sqrt(data.h));
    std::vector<float*>mat_data(data.c);
    //float *mat_data[data.c];
    for (int i = 0; i < data.c; i++) {
        mat_data[i] = data.channel(i);
    }
    float cx, cy, w, h;
    for (int shift_y = 0; shift_y < grid_size; shift_y++) {
        for (int shift_x = 0; shift_x < grid_size; shift_x++) {
            int loc = shift_x + shift_y * grid_size;
            for (int i = 0; i < 3; i++) {
                float *record = mat_data[i];
                float *cls_ptr = record + 5;
                for (int cls = 0; cls < num_classes; cls++) {
                    float score = sigmoid(cls_ptr[cls]) * sigmoid(record[4]);
                    if (score > threshold) {
                        
                        cx = (sigmoid(record[0]) * 2.f - 0.5f + (float) shift_x) * (float) stride;
                        cy = (sigmoid(record[1]) * 2.f - 0.5f + (float) shift_y) * (float) stride;
                        w = pow(sigmoid(record[2]) * 2.f, 2) * anchors[i].width;
                        h = pow(sigmoid(record[3]) * 2.f, 2) * anchors[i].height;
                        //printf("[grid size=%d, stride = %d]x y w h %f %f %f %f\n",grid_size,stride,record[0],record[1],record[2],record[3]);
                        BoxInfo box;
                        box.x1 = std::max(0, std::min(frame_size.width, int((cx - w / 2.f) * (float) frame_size.width / (float) net_size)));
                        box.y1 = std::max(0, std::min(frame_size.height, int((cy - h / 2.f) * (float) frame_size.height / (float) net_size)));
                        box.x2 = std::max(0, std::min(frame_size.width, int((cx + w / 2.f) * (float) frame_size.width / (float) net_size)));
                        box.y2 = std::max(0, std::min(frame_size.height, int((cy + h / 2.f) * (float) frame_size.height / (float) net_size)));
                        box.score = score;
                        box.label = cls;
                        result.push_back(box);
                    }
                }
            }
            for (auto &ptr:mat_data) {
                ptr += (num_classes + 5);
            }
        }
    }
    return result;
}

void YoloV5::nms(std::vector<BoxInfo> &input_boxes, float NMS_THRESH) {
    std::sort(input_boxes.begin(), input_boxes.end(), [](BoxInfo a, BoxInfo b) { return a.score > b.score; });
    std::vector<float> vArea(input_boxes.size());
    for (int i = 0; i < int(input_boxes.size()); ++i) {
        vArea[i] = (input_boxes.at(i).x2 - input_boxes.at(i).x1 + 1)
                   * (input_boxes.at(i).y2 - input_boxes.at(i).y1 + 1);
    }
    for (int i = 0; i < int(input_boxes.size()); ++i) {
        for (int j = i + 1; j < int(input_boxes.size());) {
            float xx1 = std::max(input_boxes[i].x1, input_boxes[j].x1);
            float yy1 = std::max(input_boxes[i].y1, input_boxes[j].y1);
            float xx2 = std::min(input_boxes[i].x2, input_boxes[j].x2);
            float yy2 = std::min(input_boxes[i].y2, input_boxes[j].y2);
            float w = std::max(float(0), xx2 - xx1 + 1);
            float h = std::max(float(0), yy2 - yy1 + 1);
            float inter = w * h;
            float ovr = inter / (vArea[i] + vArea[j] - inter);
            if (ovr >= NMS_THRESH) {
                input_boxes.erase(input_boxes.begin() + j);
                vArea.erase(vArea.begin() + j);
            } else {
                j++;
            }
        }
    }
}

bool YoloV5::resize_keepRatio(
    cv::Mat& src, 
    cv::Mat& dst, 
    float& scale, 
    //float& w_pad,
    //float& h_pad,
    int dstSize)
{
    float ratio_w = (float)dstSize / src.cols;
    float ratio_h = (float)dstSize / src.rows;
    cv::Mat tmpMat;
    if (ratio_w > ratio_h)
    {
        scale = ratio_h;
        cv::resize(src, tmpMat, cv::Size(0, 0), ratio_h, ratio_h, 1);
    }
    else
    {
        scale = ratio_w;
        cv::resize(src, tmpMat, cv::Size(0, 0), ratio_w, ratio_w, 1);
    }
    float w_pad_tmp = (int)dstSize - tmpMat.cols;
    float h_pad_tmp = (int)dstSize - tmpMat.rows;
    //cv::copyMakeBorder(tmpMat, dst, std::ceil(h_pad_tmp), std::floor(h_pad_tmp), std::ceil(w_pad_tmp), std::floor(w_pad_tmp), cv::BORDER_CONSTANT, 0);
    cv::copyMakeBorder(tmpMat, dst, 0, h_pad_tmp, 0, w_pad_tmp, cv::BORDER_CONSTANT, 0);

    //w_pad = std::ceil(w_pad_tmp);
    //h_pad = std::ceil(h_pad_tmp);
    return true;
}
