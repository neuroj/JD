// ライセンス: 最新のGPL

//#define _DEBUG
#include "jddebug.h"

#include "board2chcompati.h"
#include "article2chcompati.h"

#include "jdlib/miscutil.h"
#include "jdlib/miscmsg.h"
#include "jdlib/jdregex.h"

#include <sstream>

using namespace DBTREE;


#ifndef MAX
#define MAX( a, b ) ( a > b ? a : b )
#endif


#ifndef MIN
#define MIN( a, b ) ( a < b ? a : b )
#endif


Board2chCompati::Board2chCompati( const std::string& root, const std::string& path_board, const std::string& name,
    const std::string& basicauth)
    : BoardBase( root, path_board, name )
{
    set_path_dat( "/dat" );
    set_path_readcgi( "/test/read.cgi" );
    set_path_bbscgi( "/test/bbs.cgi" );
    set_path_subbbscgi( "/test/subbbs.cgi" );
    set_subjecttxt( "subject.txt" );
    set_ext( ".dat" );
    set_id( path_board.substr( 1 ) ); // 先頭の '/' を除く
    set_charset( "MS932" );

    BoardBase::set_basicauth( basicauth );
}



//
// キャッシュのファイル名が正しいか
//
bool Board2chCompati::is_valid( const std::string& filename )
{
    if( filename.find( get_ext() ) == std::string::npos ) return false;
    if( filename.length() - filename.rfind( get_ext() ) != get_ext().length() ) return false;

    unsigned int dig, n;
    MISC::str_to_uint( filename.c_str(), dig, n );
    if( dig != n ) return false;
    if( dig == 0 ) return false;
        
    return true;
}


//書き込み用クッキー作成
const std::string Board2chCompati::cookie_for_write()
{
    std::list< std::string > list_cookies = BoardBase::list_cookies_for_write();
    if( list_cookies.empty() ) return std::string();

#ifdef _DEBUG
    std::cout << "Board2chCompati::cookie_for_write\n";
#endif

    JDLIB::Regex regex;

    std::string str_expire;
    std::string query_expire = "expires=([^;]*)";

    std::string str_path;
    std::string query_path = "path=([^;]*)";

    bool use_pon = false;
    std::string str_pon;
    std::string query_pon = "PON=([^;]*)?";

    bool use_name = false;
    std::string str_name;
    std::string query_name = "NAME=([^;]*)?";

    bool use_mail = false;
    std::string str_mail;
    std::string query_mail = "MAIL=([^;]*)?";

    std::list< std::string >::iterator it = list_cookies.begin();

    // expire と path は一つ目のcookieから取得
    if( regex.exec( query_expire, (*it) ) ) str_expire = regex.str( 1 );
    if( regex.exec( query_path, (*it) ) ) str_path = regex.str( 1 );

    // その他は iterateして取得
    for( ; it != list_cookies.end(); ++it ){

        std::string tmp_cookie = MISC::strtoutf8( (*it), get_charset() );

#ifdef _DEBUG
        std::cout << tmp_cookie << std::endl;
#endif

        if( regex.exec( query_pon, tmp_cookie ) ){
            use_pon = true;
            str_pon = regex.str( 1 );
        }
        if( regex.exec( query_name, tmp_cookie ) ){
            use_name = true;
            str_name = MISC::charset_url_encode( regex.str( 1 ), get_charset() );
        }
        if( regex.exec( query_mail, tmp_cookie ) ){
            use_mail = true;
            str_mail = MISC::charset_url_encode( regex.str( 1 ), get_charset() );
        }
    }

#ifdef _DEBUG
    std::cout << "expire = " << str_expire << std::endl
              << "path = " << str_path << std::endl    
              << "pon = " << str_pon << std::endl
              << "name = " << str_name << std::endl
              << "mail = " << str_mail << std::endl;
#endif    

    std::string cookie;

    if( use_name ) cookie += "NAME=" + str_name + "; ";
    if( use_mail ) cookie += "MAIL=" + str_mail + "; ";
    if( use_pon ) cookie += "PON=" + str_pon + "; ";

    if( cookie.empty() ) return std::string();

    cookie += "expires=" + str_expire + "; path=" + str_path;

#ifdef _DEBUG
    std::cout << "cookie = " << cookie << std::endl;
#endif 

    return cookie;
}



// 新スレ作成時の書き込みメッセージ作成
const std::string Board2chCompati::create_newarticle_message( const std::string& subject,
                                                       const std::string& name, const std::string& mail, const std::string& msg )
{
    if( subject.empty() ) return std::string();
    if( msg.empty() ) return std::string();

    std::stringstream ss_post;
    ss_post.clear();
    ss_post << "bbs="      << get_id()
            << "&subject=" << MISC::charset_url_encode( subject, get_charset() );

    // 2chのhana値
    std::string hana = hana_for_write();
    if( ! hana.empty() ) ss_post << "&hana=" << hana;

    ss_post << "&time="    << time_modified()
            << "&submit="  << MISC::charset_url_encode( "新規スレッド作成", get_charset() )
            << "&FROM="    << MISC::charset_url_encode( name, get_charset() )
            << "&mail="    << MISC::charset_url_encode( mail, get_charset() )
            << "&MESSAGE=" << MISC::charset_url_encode( msg, get_charset() );

#ifdef _DEBUG
    std::cout << "Board2chCompati::create_newarticle_message " << ss_post.str() << std::endl;
#endif

    return ss_post.str();
}



//
// 新スレ作成時のbbscgi のURL
//
// (例) "http://www.hoge2ch.net/test/bbs.cgi"
//
//
const std::string Board2chCompati::url_bbscgi_new()
{
    std::string cgibase = url_bbscgibase();
    return cgibase.substr( 0, cgibase.length() -1 ); // 最後の '/' を除く
}


//
// 新スレ作成時のsubbbscgi のURL
//
// (例) "http://www.hoge2ch.net/test/subbbs.cgi"
//
const std::string Board2chCompati::url_subbbscgi_new()
{
    std::string cgibase = url_subbbscgibase();
    return cgibase.substr( 0, cgibase.length() -1 ); // 最後の '/' を除く
}



//
// 新しくArticleBaseクラスを追加してそのポインタを返す
//
// cached : HDD にキャッシュがあるならtrue
//
ArticleBase* Board2chCompati::append_article( const std::string& id, bool cached )
{
    if( empty() ) return get_article_null();

    ArticleBase* article = new DBTREE::Article2chCompati( url_datbase(), id, cached );
    if( article ) get_list_article().push_back( article );
    else return get_article_null();
    
    return article;
}



//
// subject.txt から Aarticle のリストにアイテムを追加・更新
//
void Board2chCompati::parse_subject( const char* str_subject_txt )
{
#ifdef _DEBUG
    std::cout << "Board2chCompati::parse_subject\n";
#endif 

    const int max_subject = 512;
    const char* pos = str_subject_txt;
    char str_tmp[ max_subject ];

    while( *pos != '\0' ){
        
        const char* str_id_dat;
        int lng_id_dat = 0;
        const char* str_subject;
        int lng_subject = 0;
        char str_num[ 16 ];

        // datのID取得
        str_id_dat = pos;
        while( *pos != '<' && *pos != '\0' && *pos != '\n' ) { ++pos; ++lng_id_dat; }
        
        // 壊れてる
        if( *pos == '\0' ) break;
        if( *pos == '\n' ) { ++pos; continue; }

        // subject取得
        pos += 2;
        str_subject = pos;
        while( *pos != '\0' && *pos != '\n' ) ++pos;
        --pos;
        while( *pos != '(' && *pos != '\n' && pos != str_subject_txt ) --pos;
        
        // 壊れてる
        if( *pos == '\n' || pos == str_subject_txt ){
            MISC::ERRMSG( "subject.txt is broken" );
            break;
        }
        lng_subject = MIN( ( int )( pos - str_subject ), max_subject );
        
        // レス数取得
        ++pos;
        int i = 0;
        while( *pos != ')' && *pos != '\0' && *pos != '\n' && i < 16 ) str_num[ i++ ] = *( pos++ );

        // 壊れてる
        if( *pos == '\0' ) break;
        if( *pos == '\n' ) { ++pos; continue; }

        str_num[ i ] = '\0';
        ++pos;

        // id, subject, number 取得
        memcpy( str_tmp, str_id_dat, lng_id_dat );
        str_tmp[ lng_id_dat ] = '\0';
        std::string id = MISC::remove_space( str_tmp );

        memcpy( str_tmp, str_subject, lng_subject );
        str_tmp[ lng_subject ] = '\0';
        std::string subject = MISC::remove_space( str_tmp );
        subject = MISC::replace_str( subject, "&lt;", "<" );
        subject = MISC::replace_str( subject, "&gt;", ">" );
        
        int number = atol( str_num );

#ifdef _DEBUG
        std::cout << pos - str_subject_txt << " " << lng_subject << " id = " << id << " num = " << number;
        std::cout << " : " << subject << std::endl;
#endif

        // DBに登録されてるならarticle クラスの情報更新
        ArticleBase* article = get_article( id );

        // DBにないなら新規に article クラスをDBに登録
        //
        // なおRoot::get_board()、BoardBase::read_info()経由で BoardBase::append_all_article() が既に呼ばれているので
        // DBに無いということはキャッシュに無いということ。よって append_article()の呼出に cached = false　を指定する
        if( article->empty() ) article = append_article( id,
                                                         false // キャッシュ無し
            );

        // スレ情報更新
        if( article ){

            // 情報ファイル読み込み
            article->read_info();

            // infoファイルが無い場合もあるのでsubject.txtから取得したサブジェクト、レス数を指定しておく
            article->set_subject( subject );
            article->set_number( number );

            // boardビューに表示するリスト更新
            article->set_current( true );
            if( ! BoardBase::get_abone_thread( article ) ) get_list_subject().push_back( article );
        }
    }
}
