import feedparser
import urllib.parse

def lookup(geo):
    """Looks up articles for geo."""
    
    # check cache for geo
    if geo in lookup.cache:
        return lookup.cache[geo]

    
    # get feed from Google. !!! CURRENTLY TO AVOID SPAM BOT J'ACUSE FROM GOOGLE !!!
    # http://news.google.com/news?geo=22044&output=rss
    feed = feedparser.parse("http://news.google.com/news?geo={}&output=rss".format(urllib.parse.quote(geo, safe="")))
    # feed = feedparser.parse("http://ThisIsVeryBrokenOnPurpose.broken.com/news?geo={}&output=rss".format(urllib.parse.quote(geo, safe="")))
    flag = 1
    # print(feed)
   

    # if no items in feed, get feed from Onion
    if not feed["items"]:
        feed = feedparser.parse("http://www.theonion.com/feeds/rss")
        if flag == 1:
            print("!!!!!!!!!!!!!!!!!!!!!!Google still hates you!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    

    # cache results
    lookup.cache[geo] = [{"link": item["link"], "title": item["title"]} for item in feed["items"]]

    # return results
    return lookup.cache[geo]

# initialize cache
lookup.cache = {}
