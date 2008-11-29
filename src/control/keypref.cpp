// ライセンス: GPL2

//#define _DEBUG
#include "jddebug.h"

#include "keypref.h"
#include "controlid.h"
#include "controlutil.h"


using namespace CONTROL;


//
// キーボード入力をラベルに表示するダイアログ
//
KeyInputDiag::KeyInputDiag( Gtk::Window* parent, const std::string& url,const int id )
    : CONTROL::InputDiag( parent, url, id, "ショートカットキー", INPUTDIAG_MODE_KEY )
{}


///////////////////////////////


//
// 個別のショートカットキー設定ダイアログ
//
KeyDiag::KeyDiag( Gtk::Window* parent, const std::string& url, const int id, const std::string& str_motions )
    : CONTROL::MouseKeyDiag( parent, url, id, "ショートカットキー", str_motions )
{}


InputDiag* KeyDiag::create_inputdiag()
{
    return new KeyInputDiag( this, "", get_id() );
}


const std::string KeyDiag::get_default_motions( const int id )
{
    return CONTROL::get_default_keymotions( id );
}


const std::vector< int > KeyDiag::check_conflict( const int mode, const std::string& str_motion )
{
    return CONTROL::check_key_conflict( mode, str_motion );
}


///////////////////////////////////////////////


//
// キーボード設定ダイアログ
//
KeyPref::KeyPref( Gtk::Window* parent, const std::string& url )
    : MouseKeyPref( parent, url, "ショートカットキー" )
{
    // キー設定のバックアップを取る
    // キャンセルを押したら戻す
    CONTROL::bkup_keyconfig();

    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_COMMON ) );
    append_row( CONTROL::Up );
    append_row( CONTROL::Down );

    append_row( CONTROL::Right );
    append_row( CONTROL::Left );

    append_row( CONTROL::TabRight );
    append_row( CONTROL::TabLeft );

    append_row( CONTROL::PreBookMark );
    append_row( CONTROL::NextBookMark );

    append_row( CONTROL::PrevView );
    append_row( CONTROL::NextView );

    append_row( CONTROL::ToggleArticle );

    append_row( CONTROL::ShowPopupMenu );

    append_row( CONTROL::ShowMenuBar );
    append_row( CONTROL::ShowSideBar );

    append_row( CONTROL::PageUp );
    append_row( CONTROL::PageDown );

    append_row( CONTROL::Home );
    append_row( CONTROL::End );

    append_row( CONTROL::Back );

    append_row( CONTROL::Quit );
    append_row( CONTROL::Save );
    append_row( CONTROL::Delete );
    append_row( CONTROL::Reload );
    append_row( CONTROL::StopLoading );
    append_row( CONTROL::Copy );
    append_row( CONTROL::SelectAll );
    append_row( CONTROL::AppendFavorite );

    append_row( CONTROL::Search );
    append_row( CONTROL::SearchInvert );
    append_row( CONTROL::SearchNext );
    append_row( CONTROL::SearchPrev );
    append_row( CONTROL::DrawOutAnd );

    append_comment_row( "" );
    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_EDIT ) );
    append_row( CONTROL::HomeEdit );
    append_row( CONTROL::EndEdit );

    append_row( CONTROL::UpEdit );
    append_row( CONTROL::DownEdit );
    append_row( CONTROL::RightEdit );
    append_row( CONTROL::LeftEdit );

    append_row( CONTROL::DeleteEdit );
    append_row( CONTROL::BackspEdit );
    append_row( CONTROL::UndoEdit );

    append_row( CONTROL::InputAA );

    append_comment_row( "" );
    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_BBSLIST ) );
    append_row( CONTROL::OpenBoard );
    append_row( CONTROL::OpenBoardTab );

    append_comment_row( "" );
    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_BOARD ) );
    append_row( CONTROL::OpenArticle );
    append_row( CONTROL::OpenArticleTab );
    append_row( CONTROL::NewArticle );
    append_row( CONTROL::SearchCache );

    append_row( CONTROL::ScrollRightBoard );
    append_row( CONTROL::ScrollLeftBoard );

    append_comment_row( "" );
    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_ARTICLE ) );
    append_row( CONTROL::UpMid );
    append_row( CONTROL::UpFast );

    append_row( CONTROL::DownMid );
    append_row( CONTROL::DownFast );

    append_row( CONTROL::PrevRes );
    append_row( CONTROL::NextRes );

    append_row( CONTROL::GotoNew );
    append_row( CONTROL::WriteMessage );

    append_row( CONTROL::SearchNextArticle );

    append_row( CONTROL::LiveStartStop );

    append_comment_row( "" );
    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_IMAGEVIEW ) );
    append_row( CONTROL::CancelMosaic );
    append_row( CONTROL::ZoomFitImage );
    append_row( CONTROL::ZoomInImage );
    append_row( CONTROL::ZoomOutImage );
    append_row( CONTROL::OrgSizeImage );

    append_row( CONTROL::ScrollUpImage );
    append_row( CONTROL::ScrollDownImage );
    append_row( CONTROL::ScrollLeftImage );
    append_row( CONTROL::ScrollRightImage );

    append_comment_row( "" );
    append_comment_row( "■ " + CONTROL::get_mode_label( CONTROL::MODE_MESSAGE ) );
    append_row( CONTROL::CancelWrite );
    append_row( CONTROL::ExecWrite );
    append_row( CONTROL::FocusWrite );
}


MouseKeyDiag* KeyPref::create_setting_diag( const int id, const std::string& str_motions )
{
    return new KeyDiag( this, "", id, str_motions );
}


const std::string KeyPref::get_str_motions( const int id )
{
    return CONTROL::get_str_keymotions( id );
}


const std::string KeyPref::get_default_motions( const int id )
{
    return CONTROL::get_default_keymotions( id );
}


void KeyPref::set_motions( const std::string& name, const std::string& str_motions )
{
    CONTROL::set_keymotions( name, str_motions );
}


const bool KeyPref::remove_motions( const int id )
{
    return CONTROL::remove_keymotions( id );
}


//
// キャンセルボタンを押した
//
void KeyPref::slot_cancel_clicked()
{
#ifdef _DEBUG
    std::cout << "KeyPref::slot_cancel_clicked\n";
#endif

    // キー設定を戻す
    CONTROL::restore_keyconfig();
}