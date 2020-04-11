#ifndef CPPURSES_DEMOS_GLYPH_PAINT_GLYPH_SELECT_HPP
#define CPPURSES_DEMOS_GLYPH_PAINT_GLYPH_SELECT_HPP
#include <cstddef>

#include <signals/signal.hpp>

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/button.hpp>
#include <cppurses/widget/widgets/horizontal_slider.hpp>

#include "glyph_select_stack.hpp"

namespace demos {
namespace glyph_paint {

/// Holds pages of Glyphs that can be cycled through and selected.
/** When a Glyph is selected a Signal is emitted with that Glyph sent along. */
class Glyph_select : public cppurses::layout::Vertical<> {
    struct Bottom_row : cppurses::layout::Horizontal<> {
        Bottom_row();

        cppurses::Button& left_btn =
            this->make_child<cppurses::Button>(cppurses::Glyph_string{
                L'⏴' | foreground(cppurses::Color::Black), 1});

        cppurses::Horizontal_slider& slider =
            this->make_child<cppurses::Horizontal_slider>();

        cppurses::Button& right_btn =
            this->make_child<cppurses::Button>(cppurses::Glyph_string{
                L'⏵' | foreground(cppurses::Color::Black), 1});
    };

   public:
    /// Construct with \p symbols has contents.
    explicit Glyph_select(cppurses::Glyph_string symbols = "");

    /// Set \p symbols as the contents.
    void set_symbols(cppurses::Glyph_string symbols);

    /// Emitted when a Glyph is clicked, sending along the glyph.
    sig::Signal<void(cppurses::Glyph)> glyph_selected;

   private:
    Glyph_select_stack& glyph_stack{this->make_child<Glyph_select_stack>()};
    Bottom_row& bottom_row{this->make_child<Bottom_row>()};

    std::size_t current_index();
    void flip_page_forward();
    void flip_page_backward();
};

}  // namespace glyph_paint
}  // namespace demos
#endif  // CPPURSES_DEMOS_GLYPH_PAINT_GLYPH_SELECT_HPP
