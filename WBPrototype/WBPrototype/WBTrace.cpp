#include "stdafx.h"
#include <cstdio>
#include <cstdio>
#include <streambuf>
#include <io.h>
#include <fcntl.h>
#include <sstream>
#include <sys\stat.h>

// for write()
namespace WBPrototype
{
    class fdoutbuf : public std::streambuf
    {
    protected:
        static  const int bufferSize = 256;   // size of data buffer
        char    buffer[bufferSize];            // data buffer
        const   int     m_fd;

    public:
        /* constructor
        * - initialize data buffer
        * - one character less to let the bufferSizeth character
        *    cause a call of overflow()
        */
        fdoutbuf(const int fd) : m_fd(fd)
        {
            setp(buffer, buffer + (bufferSize - 1));
        }

        /* destructor
        * - flush data buffer
        */
        virtual ~fdoutbuf()
        {
            sync();
        }

    protected:
        // flush the characters in the buffer
        int flushBuffer() 
        {
            int num = pptr() - pbase();
            const int written =  _write(m_fd, buffer, num);
            if (written != num)
            {
                return EOF;
            }
            pbump(-num);    // reset put pointer accordingly
            return num;
        }

        /* buffer full
        * - write c and all previous characters
        */
        virtual int_type overflow(int_type c)
        {
            if (c != EOF) 
            {
                // insert character into the buffer
                *pptr() = c;
                pbump(1);
            }
            // flush the buffer
            if (flushBuffer() == EOF)
            {
                // ERROR
                return EOF;
            }
            return c;
        }

        /* synchronize data with file/destination
        * - flush the data in the buffer
        */
        virtual int sync()
        {
            if (flushBuffer() == EOF) {
                // ERROR
                return -1;
            }
            return 0;
        }
    };

    int CWBTrace::Initialise(const std::string sFilename)
    {
        int fd = -1;
        const errno_t err_ = _sopen_s(&fd, sFilename.c_str(), _O_RDWR | _O_CREAT | _O_TRUNC, _SH_DENYNO, _S_IREAD | _S_IWRITE);
        
        if (err_ != 0)
        {
            std::ostringstream oss; 
            oss << "Error (" << err_ << ") occured during trace initialisation";
            const std::string s = oss.str();

            AfxMessageBox(s.c_str(), MB_OK);
            exit(err_);
        }
        else
        {
            std::streambuf *const buf = new fdoutbuf(fd);
            GetStream().rdbuf(buf);
        }

        return 0;
    }

    void CWBTrace::Cleanup()
    {
        std::ostream& s = GetStream();
        std::streambuf* buf = s.rdbuf();
        s.rdbuf(nullptr);
        delete buf;
    }

    std::ostream& CWBTrace::GetStream()
    {
        static std::ostream os(nullptr);
        return os;
    }
}