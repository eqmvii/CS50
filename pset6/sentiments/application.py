from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, 100)

    # TODO
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    analyzer = Analyzer(positives, negatives)
    
    # analyze each tweet
    posTweets, negTweets, neutralTweets = 0, 0, 0
    positive, negative, neutral = 0.0, 0.0, 0.0
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score > 0.0:
            posTweets += 1
        elif score < 0.0:
            negTweets += 1
        else:
            neutralTweets += 1
            
    positive = posTweets / len(tweets)
    negative = negTweets / len(tweets)
    neutral = neutralTweets / len(tweets)
            
    

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
