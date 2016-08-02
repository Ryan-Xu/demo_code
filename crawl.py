#!/usr/bin/env pyton

from sys import argv
from os import makedirs, unlink, sep
from os.path import dirname, exists, isdir,splitext
from string import replace, find, lower
from htmllib import HTMLParser
from urllib import urlretrieve
from urlparse import urlparse, urljoin
from formatter import DumbWriter, AbstractFormatter
from cStringIO import StringIO

class Retriever(object):
    def __init__(self, url):
        self.url = url
        self.file = self.filename(url)

    def filename(self, url, deffile='index.htm'):
        parsedurl = urlparse(url, 'http:', 0)
        path = parsedurl[1] + parsedurl[2]
        ext = splitext(path)
        if ext[1] == '': # no file, use default
            if path[-1] == '/':
                path += deffile
            else:
                path += '/' + deffile
                ldir = dirname(path) # local directory
                if sep != '/':
                    ldir = replace(ldir, '/', sep)
                if not isdir(ldir):
                    if exists(ldir): unlink(ldir)
                        makedirs(ldir)
                        return path

    def  download(self): # download Web page
        try:
            retval = urlretrieve(self.url, self.file)
        except IOError:
            retval = ('*** ERROR: invalid URL "%s"' %\
            self.url,)
        return retval

    def parseAndGetLinks(self): #parse HTML, save links
        self.parser = HTMLParser(AbstractFormatter(DumbWriter(StringIO())))
        self.parser.feed(open(self.file).read())
        self.parser.close()
        return self.parser.anchorlist


class Crawler(object): #manage entire crawling process
    count = 0 # static downloaded page conunter

    def __init__(self, url):
        self.q = [url]
        self.seen = []
        self.dom = urlparse(url)[1]

    def getPage(self, url):
        r = Retriever(url)
        retval = r.download()
        if retval[0] == '*': # error situation, do not parse
            print retval, '... skipping parse'
            return
        Crawler.count += 1
        print '\n(', Crawler.count, ')'
        print 'URL:', url
        print 'FILE:', retval[0]
        self.seen.append(url)

        link = r.parseAndGetLinks() # get and process links
        for eachLink in links:
            if eachLink[:4] != 'http' and \
                find(eachLink, '://') == -1:
                eachLink = urljoin(url, eachLink)
                print '*', eachLink

            if find(lower(eachLink), 'mailto:') != -1:
                print '... discarded, mailto link'
                continue

            if eachLink not in self.seen:
                if find(eachLink, self.dom) == -1:
                    print '... discarded, not in domain'

