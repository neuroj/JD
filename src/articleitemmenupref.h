// ライセンス: GPL2

// スレビューのコンテキストメニューの表示項目設定

#ifndef _ARTICLEITEMMENUPREF_H
#define _ARTICLEITEMMENUPREF_H

#include "skeleton/selectitempref.h"

namespace CORE
{
    class ArticleItemMenuPref : public SKELETON::SelectItemPref
    {
      public:

        ArticleItemMenuPref( Gtk::Window* parent, const std::string& url );
        virtual ~ArticleItemMenuPref(){}

      private:

        // OKボタン
        virtual void slot_ok_clicked();


        // デフォルトボタン
        virtual void slot_default();
    };
}

#endif
