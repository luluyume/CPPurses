#include "color_display.hpp"

#include <cppurses/painter/color.hpp>
#include <cppurses/widget/border.hpp>

using namespace cppurses;

namespace palette {

Color_patch::Color_patch(Color value) {
    this->set_color(value);
    this->width_policy.maximum(5);
    this->height_policy.maximum(2);
}

void Color_patch::set_color(Color value) {
    this->brush.set_background(value);
    this->update();
}

Color_display::Color_display(Color color) {
    this->height_policy.maximum(3);
    this->border.enable();
    this->border.segments.disable_all();
    this->border.segments.south.enable();
    this->set_color(color);
}

void Color_display::set_color(Color color) {
    text.name.set_contents(color_to_string(color));
    square.set_color(color);
}
}  // namespace palette
