#ifndef _SURBASIC_TIMESTAMP_H_
#define _SURBASIC_TIMESTAMP_H_
#include "surbasic/Type.h"
#include <sys/time.h>

namespace SDBasic{
namespace time{
struct Date {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
  unsigned int us;
};

class Timestamp {
 public:
  static const int64_t kMiliSecondsPerSecond = 1000L;
  static const int64_t kMicroSecondsPerSecond = 1000L*1000L;
  static const int64_t kNanoSecondsPerMicroSecond = 1000L;

  Timestamp(): us_since_epoch_(0) { }
  explicit Timestamp(int64_t raw): us_since_epoch_(raw) { }
  Timestamp(const Timestamp& other): us_since_epoch_(other.us_since_epoch_) { }

  void swap(Timestamp& other) {
    std::swap(us_since_epoch_, other.us_since_epoch_);
  }

  static Timestamp Now();
  static Timestamp AfterNSeconds(double seconds);
  static Timestamp AfterNMicroseconds(double microsends);
  static Timestamp Epoch();

  static Timestamp FromISO8601(const std::string &s);
  std::string ToISO8601() const;

  static Timestamp FromTimespec(const struct timespec& spec);
  struct timeval ToTimeval() const;
  struct timespec ToTimespec() const;

  time_t ToTimeT() const;
  static Timestamp FromTimeT(time_t t);

  std::string ToLogTime() const;

  bool Valid() const { return us_since_epoch_ != -1; }

  int64_t MicroSecondsSinceEpoch() const { return us_since_epoch_; }

  void Breakdown(Date * b) const;

  bool Expired() const;

  inline bool operator == (const Timestamp& other) const;
  inline bool operator != (const Timestamp& other) const;
  inline bool operator >= (const Timestamp& other) const;
  inline bool operator >  (const Timestamp& other) const;
  inline bool operator <= (const Timestamp& other) const;
  inline bool operator <  (const Timestamp& other) const;


 private:
  int64_t us_since_epoch_;
};

bool Timestamp::operator == (const Timestamp& other) const {
  return MicroSecondsSinceEpoch() == other.MicroSecondsSinceEpoch();
}

bool Timestamp::operator != (const Timestamp& other) const {
  return MicroSecondsSinceEpoch() != other.MicroSecondsSinceEpoch();
}

bool Timestamp::operator >= (const Timestamp& other) const {
  return MicroSecondsSinceEpoch() >= other.MicroSecondsSinceEpoch();
}

bool Timestamp::operator >  (const Timestamp& other) const {
  return MicroSecondsSinceEpoch() >  other.MicroSecondsSinceEpoch();
}

bool Timestamp::operator <= (const Timestamp& other) const {
  return MicroSecondsSinceEpoch() <= other.MicroSecondsSinceEpoch();
}

bool Timestamp::operator <  (const Timestamp& other) const {
  return MicroSecondsSinceEpoch() <  other.MicroSecondsSinceEpoch();
}

inline double TimeDiffInSeconds(Timestamp t1, Timestamp t2) {
  int64_t diff = t1.MicroSecondsSinceEpoch() - t2.MicroSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

}}

#endif // _SURBASIC_TIMESTAMP_H_
