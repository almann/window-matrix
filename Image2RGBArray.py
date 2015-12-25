#!/usr/bin/python
import sys          as _sys
import time         as _time
import PIL.Image    as _image

def main() :
    print '/* Generated by Image2RGBArray on %s */' % _time.asctime()
    print
    print 'static const RGB kImages[][kMatrixPixels] = {'
    for name in _sys.argv[1:] :
        print '  {'
        print >> _sys.stderr, 'Loading: %s' % name
        im = _image.open(name, 'r')
        pix = im.load()
        cols, rows = im.size
        for x in xrange(cols) :
            for y in xrange(rows) :
                r, g, b = pix[x, y]
                print '    {%d, %d, %d},' % (r, g, b)
        print '  },'

    print '};'
    print
    print 'static const size_t kImageCount = %d;' % (len(_sys.argv) - 1)


if __name__ == '__main__' :
    main()