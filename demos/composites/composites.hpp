#ifndef CPPURSES_DEMOS_COMPOSITES_COMPOSITES_HPP
#define CPPURSES_DEMOS_COMPOSITES_COMPOSITES_HPP
#include <utility>

#include <cppurses/widget/array.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/pipe.hpp>
#include <cppurses/widget/tuple.hpp>
#include <cppurses/widget/widget.hpp>
#include <cppurses/widget/widgets/button.hpp>
#include <cppurses/widget/widgets/textbox.hpp>

namespace comp {

// TODO namespace for these policy(?) types?

// template <typename Widget_t>
// struct Border : Widget_t {
//     Border() { this->border.enable(); }
// };

// template <typename Widget_t>
// struct Border_east : Widget_t {
//     Border_east()
//     {
//         this->border.enable();
//         this->border.segments.disable_all();
//         this->border.segments.east.enable();
//     }
// };

// template <typename Widget_t>
// struct Border_west : Widget_t {
//     Border_west()
//     {
//         this->border.enable();
//         this->border.segments.disable_all();
//         this->border.segments.west.enable();
//     }
// };

// template <typename Widget_t>
// struct Border_north : Widget_t {
//     Border_north()
//     {
//         this->border.enable();
//         this->border.segments.disable_all();
//         this->border.segments.north.enable();
//     }
// };

// template <typename Widget_t>
// struct Border_south : Widget_t {
//     Border_south()
//     {
//         this->border.enable();
//         this->border.segments.disable_all();
//         this->border.segments.south.enable();
//     }
// };

// template <typename Widget_t, cppurses::Color Bg>
// struct Background : Widget_t {
//     Background() { this->brush.set_background(Bg); }
// };

// using Foo = cppurses::Tuple<
//     cppurses::layout::Horizontal<>,
//     cppurses::Button,
//     Background<cppurses::Button, cppurses::Color::Orange>,
//     Background<Border_east<cppurses::Textbox>, cppurses::Color::Violet>,
//     cppurses::Button>;

// struct Composites : Foo {
//     Composites()
//     {
//         auto& btn_1 = this->get<0>();
//         auto& btn_2 = this->get<1>();
//         auto& txbx  = this->get<2>();
//         auto& btn_3 = this->get<3>();

//         using namespace cppurses;
//         btn_1.brush.set_background(Color::Light_blue);
//         btn_1.set_label(L"X");
//         btn_2.set_label(L"Y");
//         txbx.set_contents("HeLLO");
//         btn_3.set_label(L"Z");
//     }
// };

// TODO
// SFINAE so it doesn't ... make member of Widget?
// but then you can't return a more derived type.
// TODO need namespace for all of this
auto label(cppurses::Glyph_string l)
{
    return [label = std::move(l)](auto& w) -> auto&
    {
        w.set_label(std::move(label));
        return w;
    };
}

// auto border()
// {
//     return [](auto& w) -> auto&
//     {
//         w.order.enable();
//         return w;
//     };
// }

// auto fixed_width(std::size_t amount)
// {
//     return [amount](auto& w) -> auto&
//     {
//         w.width_policy.fixed(amount);
//         return w;
//     };
// }

// auto fixed_height(std::size_t amount)
// {
//     return [amount](auto& w) -> auto&
//     {
//         w.height_policy.fixed(amount);
//         return w;
//     };
// }

// struct Btn : cppurses::layout::Horizontal<> {
//     cppurses::Button& btn =
//         this->make_child<cppurses::Button>() | label("btn") |
//         comp::background(cppurses::Color::Red) | comp::border();
// };

using Btns =
    cppurses::Array<cppurses::layout::Horizontal<>, cppurses::Button, 2>;

using App =
    cppurses::Tuple<cppurses::layout::Vertical<>, cppurses::Textbox, Btns>;

struct Composites : App {
    Composites()
    {
        auto& btns = this->get<1>();
        auto& save = btns.get<0>();
        auto& load = btns.get<1>();
        auto& txbx = this->get<0>();

        using namespace cppurses;
        using namespace cppurses::pipe;
        btns | fixed_height(1);
        save | label("Save") | bg(Color::Light_blue);
        load | label("Load") | bg(Color::Light_green);
        txbx | bg(Color::White) | fg(Color::Black);

        // *this | children() | fixed_height(4);
        *this | descendants() | bg(Color::Blue);

        save | remove_background();  // TODO maybe clear_bg?
        txbx | remove_foreground();

        // save | add_attr(Attribute::Bold, Attribute::Underline);
        // save.brush.add_attributes(Attribute::Bold, Attribute::Underline);
        save | add(Attribute::Bold) | add(Attribute::Underline);
        load | add(Attribute::Bold) | add(Attribute::Underline);

        // TODO change Attribute to Trait
        // save | add(Trait::Bold) | add(Trait::Underline);
        // save | add(Trait::Bold | Trait::Underline);

        // save | remove_attr(Attribute::Underline);
        save | remove(Attribute::Underline);
        save | clear_attributes();
        save | on_enable([&txbx]() { txbx.set_contents("Save Enabled"); });
        save | on_child_added([](Widget& w) { w.border.enable(); });

        load | on_move([&txbx](auto const& n, auto const&) {
            txbx.set_contents(std::to_string(n.x) + " " + std::to_string(n.y));
        });
        load | on_resize([&txbx](auto const& n, auto const&) {
            txbx.set_contents(std::to_string(n.width) + " " +
                              std::to_string(n.height));
        });

        load | on_mouse_press([&](auto) { load | bg(Color::Light_gray); });
        load | on_mouse_release([&](auto) { load | bg(Color::Dark_gray); });
        txbx | on_key_press([&](auto) { txbx | bg(Color::Light_blue); });

        using namespace std::literals;
        save | animate([]() { return 34ms; }) | wallpaper(L'X') |
            wallpaper(std::nullopt);
        load | wallpaper({L'-', foreground(Color::Red)});
        load | wallpaper_without_brush() | wallpaper_with_brush();

        load | show_cursor() | put_cursor({2, 5});
        save | hide_cursor();
        // txbx | no_focus();
        load | strong_focus() | bg(Color::Green);

        // save | fixed_width(5);
        // save | minimum_width(2);
        // save | maximum_width(7);
        // save | preferred_width(7);
        // save | expanding_width(7);
        // save | minimum_expanding_width(7);
        // save | ignored_width();

        // save | width_hint(2);
        // save | width_min(2);
        // save | width_max(2);
        // save | width_stretch(2.3);
        // save | can_ignore_width_min();
        // save | cannot_ignore_width_min();

        // save | fixed_height(5);
        // save | minimum_height(2);
        // save | maximum_height(7);
        // save | preferred_height(7);
        // save | expanding_height(7);
        // save | minimum_expanding_height(7);
        // save | ignored_height();

        // save | height_hint(2);
        // save | height_min(2);
        // save | height_max(2);
        // save | height_stretch(2.3);
        // save | can_ignore_height_min();
        // save | cannot_ignore_height_min();

        // txbx | bordered() | block_walls_4() | west_wall(L'▓')
        //      | north_wall(L'▓') | north_west_corner(L'▓');

        // txbx | bordered() | block_walls_3() | north_west_walls(L'▓');
        txbx | bordered() | asterisk_walls() | plus_corners();
        load | on_mouse_press([&](auto const& m) {
            if (m.modifiers.ctrl)
                txbx | bg(Color::Orange);
        });
        // constexpr auto g = L'G' | Attribute::Bold;
        auto gs = "hello" | Attribute::Inverse;
    }
};

}  // namespace comp
#endif  // CPPURSES_DEMOS_COMPOSITES_COMPOSITES_HPP
