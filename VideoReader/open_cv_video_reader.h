#ifndef OPEN_CV_VIDEO_READER_H
#define OPEN_CV_VIDEO_READER_H
#include "video_reader.h"

using namespace submodule_video_reader;

class open_cv_video_reader : public abstract_video_reader
{
protected:
  std::string m_path_to_file;
  uint64_t m_frame_count;
  VideoCapture *m_cap;

public:
  open_cv_video_reader(std::string path_to_video);
  ~open_cv_video_reader();

  // abstract_video_reader interface
public:
  uint64_t get_current_frame_num();

  void set_logger(abstract_logger *logger);

  Mat get_current_frame();
  Mat get_previous_frame();

  Mat read_frame_by_num(uint64_t num);
  Mat read_next_frame();

  bool is_finished();
  bool is_started();

  void reset();
  void stop();
};


#endif // OPEN_CV_VIDEO_READER_H
