//
// Created by 邓昊晴 on 14/6/2020.
//

#ifndef YOLOV5_H
#define YOLOV5_H

#include "net.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
namespace yolocv {
    typedef struct {
        int width;
        int height;
    } YoloSize;
}

typedef struct {
    std::string name;
    int stride;
    std::vector<yolocv::YoloSize> anchors;
} YoloLayerData;

typedef struct BoxInfo {
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;
} BoxInfo;

class YoloV5 {
public:
    YoloV5(const char *param, const char *bin, bool useGPU);
    ~YoloV5();
    //"person", "bicycle", "car", "motorcycle", "airplane", "bus"
    // "cctv1", "guangdong", "hunan", "shanghai", "youku", "zhejiang", "train",
    //"hand1", "hand2", "hand3", "hand4",
    std::vector<std::string> labels{ "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
                                    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
                                    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                                    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                                    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                                    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
                                    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
                                    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
                                    "hair drier", "toothbrush"};
    //std::vector<std::string> labels{ "hand1", "hand2", "hand3", "hand4" };
public:
    //std::vector<BoxInfo> detect(cv::Mat& image, float threshold, float nms_threshold);
    bool detect(cv::Mat& image, float threshold, float nms_threshold);
    bool draw(cv::Mat& image);
    std::vector<BoxInfo> detect_result;//检测结果
    float scale_input;//输入图像缩放比例
    //float hPad; //x向, 单侧填充的数值
    //float wPad; //y向, 单侧填充的数值


private:
    static std::vector<BoxInfo>
    decode_infer(ncnn::Mat &data, int stride, const yolocv::YoloSize &frame_size, int net_size, int num_classes,
                 const std::vector<yolocv::YoloSize> &anchors, float threshold);

    static void nms(std::vector<BoxInfo> &result, float nms_threshold);

    ncnn::Net *Net;
    int input_size = 640;
    int num_class = 80; //80
    //std::vector<YoloLayerData> layers{
    //    {"out2",32, {{116, 90}, {156, 198}, {373, 326}}},
    //    {"out1",16, {{30,  61}, {62,  45},  {59,  119}}},
    //    {"out0",8,  {{10,  13}, {16,  30},  {33,  23}}},
    //};
    std::vector<YoloLayerData> layers{
            {"367",     32, {{116, 90}, {156, 198}, {373, 326}}},
            {"353",     16, {{30,  61}, {62,  45},  {59,  119}}},
            {"output",  8,  {{10,  13}, {16,  30},  {33,  23}}},
    };
    //std::vector<YoloLayerData> layers{
    //    {"394",     32, {{116, 90}, {156, 198}, {373, 326}}},
    //    {"375",     16, {{30,  61}, {62,  45},  {59,  119}}},
    //    {"output",  8,  {{10,  13}, {16,  30},  {33,  23}}},
    //};
public:
    static YoloV5 *detector;
    static bool hasGPU;

public:
    /*
    * 保持宽高比的缩放
    * src:input
    * dst:output
    * scale:output,缩放比例
    * w_pad,x向,单侧填充的数值
    * h_pad,y向,单侧填充的数值
    * dstSize:input,目标尺寸
    */
    static bool resize_keepRatio(
        cv::Mat& src,
        cv::Mat& dst,
        float& scale,
        //float& w_pad,
        //float& h_pad,
        int dstSize
    );
};


#endif //YOLOV5_H
