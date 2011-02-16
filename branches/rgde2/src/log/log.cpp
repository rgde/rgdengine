#include "stdafx.h"

#include <rgde/log/log.h>
#include <rgde/log/filter.h>

#include "log_helper.h"

namespace rgde
{
    namespace log
    {
        //безымянное пространство имен для сокрытия глобальных переменных
        namespace
        {
            //вспомогательные глобальные переменные (обьекты, куда направлен вывод сообщенйи лога)
            device::PFileWrapper     pfilewrapper     (new device::file_wrapper());
            device::POStreamWrapperA postreamwrapperA (new device::ostream_wrapper<char>());
            device::POStreamWrapperW postreamwrapperW (new device::ostream_wrapper<wchar_t>());

            device::file_dev  <char> fdev(pfilewrapper);
            device::screen_dev<char> sdev(postreamwrapperA);
            device::output_dev<char> odev;

            device::file_dev  <wchar_t> wfdev(pfilewrapper);
            device::screen_dev<wchar_t> wsdev(postreamwrapperW);
            device::output_dev<wchar_t> wodev;

            //потоки для вывода сообщений в лог
            typedef boost::iostreams::filtering_stream<boost::iostreams::output> dev_stream;

            dev_stream lmsg_file   (filter::html_msg<char>() | filter::html_time<char>() | fdev );
            dev_stream lmsg_screen (filter::text_msg<char>() | filter::text_time<char>() | sdev );
            dev_stream lmsg_output (filter::text_msg<char>() | filter::text_time<char>() | odev );

            dev_stream lwrn_file   (filter::html_wrn<char>() | filter::html_time<char>() | fdev );
            dev_stream lwrn_screen (filter::text_wrn<char>() | filter::text_time<char>() | sdev );
            dev_stream lwrn_output (filter::text_wrn<char>() | filter::text_time<char>() | odev );

            dev_stream lerr_file   (filter::html_err<char>() | filter::html_time<char>() | fdev );
            dev_stream lerr_screen (filter::text_err<char>() | filter::text_time<char>() | sdev );
            dev_stream lerr_output (filter::text_err<char>() | filter::text_time<char>() | odev );

            typedef boost::iostreams::wfiltering_stream<boost::iostreams::output> wdev_stream;

            wdev_stream wlmsg_file   (filter::html_msg<wchar_t>() | filter::html_time<wchar_t>() | wfdev );
            wdev_stream wlmsg_screen (filter::text_msg<wchar_t>() | filter::text_time<wchar_t>() | wsdev );
            wdev_stream wlmsg_output (filter::text_msg<wchar_t>() | filter::text_time<wchar_t>() | wodev );

            wdev_stream wlwrn_file   (filter::html_wrn<wchar_t>() | filter::html_time<wchar_t>() | wfdev );
            wdev_stream wlwrn_screen (filter::text_wrn<wchar_t>() | filter::text_time<wchar_t>() | wsdev );
            wdev_stream wlwrn_output (filter::text_wrn<wchar_t>() | filter::text_time<wchar_t>() | wodev );

            wdev_stream wlerr_file   (filter::html_err<wchar_t>() | filter::html_time<wchar_t>() | wfdev );
            wdev_stream wlerr_screen (filter::text_err<wchar_t>() | filter::text_time<wchar_t>() | wsdev );
            wdev_stream wlerr_output (filter::text_err<wchar_t>() | filter::text_time<wchar_t>() | wodev );

            //"устройства" для вывода сообщений
            device::composite_dev<char> msg_dev(lmsg_file, lmsg_screen, lmsg_output);
            device::composite_dev<char> wrn_dev(lwrn_file, lwrn_screen, lwrn_output);
            device::composite_dev<char> err_dev(lerr_file, lerr_screen, lerr_output);

            device::composite_dev<wchar_t> wmsg_dev(wlmsg_file, wlmsg_screen, wlmsg_output);
            device::composite_dev<wchar_t> wwrn_dev(wlwrn_file, wlwrn_screen, wlwrn_output);
            device::composite_dev<wchar_t> werr_dev(wlerr_file, wlerr_screen, wlerr_output);
        }

        //потоки для вывода сообщений
        log_stream lmsg(msg_dev);
        log_stream lwrn(wrn_dev);
        log_stream lerr(err_dev);

        wlog_stream wmsg(wmsg_dev);
        wlog_stream wwrn(wwrn_dev);
        wlog_stream werr(werr_dev);

        int logsystem::counter = 0;
        logsystem::loghelper* logsystem::ploghelper = 0;

        logsystem::logsystem()
        {
            if (counter++ == 0)
                ploghelper = new loghelper();
        }

        logsystem::~logsystem()
        {
            if (--counter == 0)
                delete ploghelper;
        }

        logsystem::loghelper::loghelper():
            coutsbuf  (0),
            wcoutsbuf (0),
            cerrsbuf  (0),
            wcerrsbuf (0)
        {
            _wsetlocale(LC_COLLATE, L"rus"); 
            _wsetlocale(LC_CTYPE, L"rus"); 
            _wsetlocale(LC_TIME, L"rus");
            _wsetlocale(LC_NUMERIC, L"English");

            pfilewrapper->open(generateLogName());
            postreamwrapperA->open(std::cout.rdbuf());
            postreamwrapperW->open(std::wcout.rdbuf());

            //(w)cout
            coutsbuf = std::cout.rdbuf();
            std::cout.rdbuf(lmsg.rdbuf());

            wcoutsbuf = std::wcout.rdbuf();
            std::wcout.rdbuf(wmsg.rdbuf());

            //(w)cerr
            cerrsbuf = std::cerr.rdbuf();
            std::cerr.rdbuf(lerr.rdbuf());
            std::cerr.unsetf(std::ios_base::unitbuf);

            wcerrsbuf = std::wcerr.rdbuf();
            std::wcerr.rdbuf(werr.rdbuf());
            std::wcerr.unsetf(std::ios_base::unitbuf);
        }

        logsystem::loghelper::~loghelper()
        {
            //(w)err
            std::wcerr.setf(std::ios_base::unitbuf);
            std::wcerr.rdbuf(wcerrsbuf);
            std::cerr.setf(std::ios_base::unitbuf);
            std::cerr.rdbuf(cerrsbuf);

            //(w)cout
            std::wcout.rdbuf(wcoutsbuf);
            std::cout.rdbuf(coutsbuf);

            postreamwrapperW->close();
            postreamwrapperA->close();
            pfilewrapper->close();
        }

        std::string logsystem::loghelper::generateLogName()
        {
            std::string strLogName;
            strLogName.append("RGDE_Log_");
            strLogName.append(util::get_current_datetime().c_str());
            strLogName.append(".html");

            return strLogName;
        }
    }
}