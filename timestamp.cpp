#include "surbasic/timestamp.h"
#include <sys/time.h>

namespace SDBasic {
namespace time{
Timestamp Timestamp::Now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::FromTimespec(const struct timespec& spec) {
  return Timestamp(spec.tv_sec * kMicroSecondsPerSecond + 
                   spec.tv_nsec/kNanoSecondsPerMicroSecond);
}

struct timeval Timestamp::ToTimeval() const {
  struct timeval tv;
  tv.tv_sec = us_since_epoch_ / kMicroSecondsPerSecond;
  tv.tv_usec = us_since_epoch_ % kMicroSecondsPerSecond;
  return tv;
}

struct timespec Timestamp::ToTimespec() const {
  struct timespec ts;
  ts.tv_sec = us_since_epoch_ / kMicroSecondsPerSecond;
  ts.tv_nsec = us_since_epoch_ % kMicroSecondsPerSecond * kNanoSecondsPerMicroSecond;
  return ts;
}

time_t Timestamp::ToTimeT() const {
  return (us_since_epoch_ / kMicroSecondsPerSecond);
}

Timestamp Timestamp::FromTimeT(time_t t) {
  return Timestamp(t * kMicroSecondsPerSecond);
}

Timestamp Timestamp::AfterNSeconds(double seconds) {
  return AfterNMicroseconds(seconds * kMicroSecondsPerSecond);
}

Timestamp Timestamp::AfterNMicroseconds(double microsends) {
  Timestamp t = Timestamp::Now();
  t.us_since_epoch_ += static_cast<int64_t>(microsends);
  return t;
}

Timestamp Timestamp::Epoch() {
  return Timestamp(0);
}

Timestamp Timestamp::FromISO8601(const std::string &s) {
  int year     = 0;
  int month    = 0;
  int days     = 0;
  int hours    = 0;
  int minutes  = 0;
  int seconds  = 0;
  int millisecond = 0;

  bool success = true;
  int count = sscanf(s.c_str(), "%d-%d-%dT%d:%d:%d.%dZ", &year, &month, &days,
                     &hours, &minutes, &seconds, &millisecond);
  success = (count == 7);

  if (success) {	
    struct tm tm;
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = days;
    tm.tm_hour = hours;
    tm.tm_min = minutes;
    tm.tm_sec = seconds;
    time_t seconds_since_epoch = timegm(&tm);
    return Timestamp(seconds_since_epoch * kMicroSecondsPerSecond + millisecond*1000);
  } 
  return Timestamp(0);
}

std::string Timestamp::ToISO8601() const {
  Date d;
  Breakdown(&d);
  char buf[32] = {0};
  snprintf(buf, sizeof(buf), "%4d-%02d-%02dT%02d:%02d:%02d.%03dZ",
           d.year, d.month, d.day, d.hour, d.min, d.sec, d.us/1000);
  return buf;
}

std::string Timestamp::ToLogTime() const {
  Date d;
  Breakdown(&d);
  char buf[32] = {0};
  snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%03dZ",
           d.year, d.month, d.day, d.hour, d.min, d.sec, d.us);
  return buf;
}

void Timestamp::Breakdown(Date* b) const { 
  struct tm tm;
  time_t seconds = static_cast<time_t>(us_since_epoch_ / kMicroSecondsPerSecond);
  gmtime_r(&seconds, &tm);

  b->year = tm.tm_year + 1900;
  b->month = tm.tm_mon + 1;
  b->day = tm.tm_mday;
  b->hour = tm.tm_hour;
  b->min = tm.tm_min;
  b->sec = tm.tm_sec;
  b->us = static_cast<unsigned int>(us_since_epoch_ % kMiliSecondsPerSecond/*kMicroSecondsPerSecond*/);
}

bool Timestamp::Expired() const {
  return (*this < Timestamp::Now());
}

}}
