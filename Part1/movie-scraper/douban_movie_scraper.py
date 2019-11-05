import urllib.request
import re
import os
import csv
import codecs
import time

def getHtml(url):
    page = urllib.request.urlopen(url);
    html = page.read();
    return html;

def getUrl(html):
    urlList = re.findall(r'"(https://movie.douban.com/subject/.*?/).*?"', html, re.S);
    urlList = list(set(urlList))
    print("Get %d urls ..."%len(urlList))
    return urlList

def getUrlList():
    urls = []

    def dfs(url, d, maxd):
        if d==maxd:
            return []
        html = getHtml(url).decode("UTF-8");
        curUrls = getUrl(html)
        for url in curUrls:
            if url not in urls:
                urls.append(url)
                dfs(url, d+1, maxd)
        return curUrls

    for page in range(0,250,25):
        url = "https://movie.douban.com/top250?start={}".format(page)
        dfs(url, d = 0, maxd = 3);

    return urls

def scrape(urls):
    for i, url in enumerate(urls):
        
        while True:
            try:
                time.sleep(3)
                html = getHtml(url).decode("UTF-8")
                break
            except:
                time.sleep(60)
                continue

        f = codecs.open("movies/%d.html"%i, "w", "utf-8")
        print(i, url)
        f.write(html)
        f.close()

def main():
    urls = getUrlList()
    scrape(urls)
  
if __name__== "__main__":
    main()
