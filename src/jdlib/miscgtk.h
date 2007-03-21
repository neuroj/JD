// ライセンス: GPL2

// gtk, gdk 関係の関数

#ifndef _MISCGTK_H
#define _MISCGTK_H

#include <gtkmm.h>

namespace MISC
{
    // Gdk::Color -> 16進数表記の文字列
    std::string color_to_str( const Gdk::Color& color );

    // int[3] -> 16進数表記の文字列
    std::string color_to_str( const int* l_rgb );

    // Gdk::Color -> int 変換
    guint32 color_to_int( const Gdk::Color& color );
}

#endif
