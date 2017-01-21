//
// PROJECT:         Aspia Remote Desktop
// FILE:            desktop_capture/desktop_point.cpp
// LICENSE:         See top-level directory
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "desktop_capture/desktop_point.h"

namespace aspia {

DesktopPoint::DesktopPoint() :
    x_(0),
    y_(0)
{
    // Nothing
}

DesktopPoint::DesktopPoint(int32_t x, int32_t y) :
    x_(x),
    y_(y)
{
    // Nothing
}

DesktopPoint::DesktopPoint(const DesktopPoint &point) :
    x_(point.x_),
    y_(point.y_)
{
    // Nothing
}

DesktopPoint::~DesktopPoint()
{
    // Nothing
}

int32_t DesktopPoint::x() const
{
    return x_;
}

int32_t DesktopPoint::y() const
{
    return y_;
}

void DesktopPoint::Set(int32_t x, int32_t y)
{
    x_ = x;
    y_ = y;
}

bool DesktopPoint::IsEqualTo(const DesktopPoint &other) const
{
    return (x_ == other.x_ && y_ == other.y_);
}

void DesktopPoint::Translate(int32_t x_offset, int32_t y_offset)
{
    x_ += x_offset;
    y_ += y_offset;
}

DesktopPoint& DesktopPoint::operator=(const DesktopPoint &other)
{
    Set(other.x_, other.y_);
    return *this;
}

bool DesktopPoint::operator==(const DesktopPoint &other)
{
    return IsEqualTo(other);
}

bool DesktopPoint::operator!=(const DesktopPoint &other)
{
    return !IsEqualTo(other);
}

} // namespace aspia
