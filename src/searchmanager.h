// ライセンス: GPL2

//
// ログ、スレタイ検索クラス
//

#ifndef _SEARCHMANAGER_H
#define _SEARCHMANAGER_H

#include "skeleton/dispatchable.h"

#include <gtkmm.h>
#include "jdlib/jdthread.h"

#include <string>
#include <list>

namespace DBTREE
{
    class ArticleBase;
}

namespace CORE
{
    class SearchLoader;

    struct SEARCHDATA
    {
        std::string url_readcgi;
        std::string boardname;
        std::string subject;
        int num;
    };

    class Search_Manager : public SKELETON::Dispatchable
    {
        typedef sigc::signal< void, const std::string& > SIG_SEARCH_FIN;

        SIG_SEARCH_FIN m_sig_search_fin;

        JDLIB::Thread m_thread;

        std::string m_id;
        std::string m_url;
        std::string m_query;
        bool m_mode_or;
        bool m_searchall;
        bool m_calc_data;

        std::vector< DBTREE::ArticleBase* > m_list_article;
        std::list< SEARCHDATA > m_list_data;

        // 検索実行中
        bool m_searching;

        bool m_stop;

        // スレタイ検索ローダ
        SearchLoader* m_searchloader;

      public:

        Search_Manager();
        virtual ~Search_Manager();

        SIG_SEARCH_FIN sig_search_fin(){ return m_sig_search_fin; }

        const std::vector< DBTREE::ArticleBase* >& get_list_article() const{ return m_list_article; }
        const std::list< SEARCHDATA >& get_list_data() const { return m_list_data; }

        const bool is_searching() const { return m_searching; }
        const bool is_searching( const std::string& id ) const { if( id == m_id ) return m_searching; else return false;  }
        void stop( const std::string& id );


        // ログ検索
        // 結果は m_list_article と m_list_data に入る。
        //
        // id : 呼び出し元の ID。 検索終了時に SIG_SEARCH_FIN シグナルで送る
        // url: ログ検索先の板のアドレス
        // query : 検索文字列、空文字ならキャッシュにあるスレを全て選択
        // mode_or : false なら AND、true なら OR で検索する
        // searchall : true なら全板検索
        // calc_data : 検索終了時に m_list_data を求める
        const bool search_log( const std::string& id, const std::string& url, const std::string& query,
                               const bool mode_or, const bool searchall, const bool calc_data );


        // スレタイ検索
        const bool search_title( const std::string& id, const std::string& query );


      private:

        static void* launcher( void* );
        void wait();
        void thread_search();
        virtual void callback_dispatch();
        void search_fin();
        void search_fin_title();
    };

    ///////////////////////////////////////
    // インターフェース

    Search_Manager* get_search_manager();
    void delete_search_manager();
}


#endif
