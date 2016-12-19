#ifndef RESIZE_EVENT_HPP
#define RESIZE_EVENT_HPP

#include "../event.hpp"

#include <cstddef>

namespace mcurses {

class Resize_event : public Event {
   public:
    Resize_event(std::size_t new_width,
                 std::size_t new_height,
                 std::size_t old_width = 0,
                 std::size_t old_height = 0);

    std::size_t old_width() const { return old_width_; }
    std::size_t old_height() const { return old_height_; }
    std::size_t new_width() const { return new_width_; }
    std::size_t new_height() const { return new_height_; }

   protected:
    std::size_t new_width_;
    std::size_t new_height_;
    std::size_t old_width_;
    std::size_t old_height_;
};

}  // namespace mcurses
#endif  // RESIZE_EVENT_HPP