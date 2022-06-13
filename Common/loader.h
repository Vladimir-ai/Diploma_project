#ifndef LOADER_H
#define LOADER_H
#include <vector>
#include <string>

class AbstractInfoQtFrame;

const static std::string g_video_reader_symbol = "_odometry_load_video_reader";
const static std::string g_feature_detector_symbol = "_odometry_load_feature_detector";
const static std::string g_feature_tracker_symbol = "_odometry_load_feature_tracker";
const static std::string g_pose_estimator_symbol = "_odometry_load_pose_estimator";

std::vector<AbstractInfoQtFrame *> read_all_files(const std::string path, const std::vector<std::string>& symbols);
std::vector<AbstractInfoQtFrame *> read_all_files(const std::string path, const std::string& symbol);

#endif /* LOADER_H */
