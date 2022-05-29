#ifndef OPEN_CV_VIDEO_READER_H
#define OPEN_CV_VIDEO_READER_H

#include "video_reader.h"


using namespace SubmoduleVideoReader;

class OpencvVideoReader : public IAbstractVideoReader
{
protected:
  std::string m_path_to_file;
  uint64_t m_frame_count;
  VideoCapture *m_cap;

public:
  OpencvVideoReader(std::string path_to_video);
  ~OpencvVideoReader();

  // abstract_video_reader interface
public:
  uint64_t get_current_frame_num();

  void set_logger(AbstractLogger *logger);

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
