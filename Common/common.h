#ifndef COMMON_H
#define COMMON_H

#ifdef QT_ASSERTION
#include <QtGlobal>
#define ASSERT(cond)    Q_ASSERT(cond)
#endif /* Q_ASSERT */

/* Use 100 interval */
enum submodule_type
{
  UNKNOWN = 0,
  /* Video reader start */
  OPENCV_READER = 100,
  /* Video reader end */

  /* Feature detector start */
  FAST_DETECTOR = 200,
  /* Feature detector end */

  /* Feature tracker start */
  OPENCV_FEATURE_TRACKER = 300,
  /* Feature tracker end */

  /* Running mode */
  STEP_BY_STEP = 400,
  CONTINUOUSLY = 401
};

#endif // COMMON_H
