import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        self.posWords = []
        fPos = open(positives, 'r')
        for line in fPos:
            if (line.startswith(";") == False):
                # print (line.strip(' ') + " -- ")
                self.posWords.append(line.rstrip())
        fPos.close()
                
        self.negWords = []
        fNeg = open(negatives, 'r')
        for line in fNeg:
            if (line.startswith(";") == False):
                # print (line.strip(' ') + " -- ")
                self.negWords.append(line.rstrip())
                
        # print(self.posWords[2])

        # Use a list, dict, or set
        # store in/as self.positives or self.negatives
        # omit leading/trailing whitespace via str.strip
        # str.startswith to avoid ; 

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        count = 0
        tknzr = nltk.TweetTokenizer()
        tokens = tknzr.tokenize(text)
        # print(tokens)
        
        # with open("file.txt") as lines:
            # for line in lines: 
            # TODO
        print("(2) Analyzing tweet... ", end='')    
        # Use str.lower 
        # if token in self.positives, 1. Negatives, -1. Else, 0.
        for word in tokens:
            if (word.lower() in self.posWords):
                count +=1
                print(word + " is a happy word... ", end='')
            if (word.lower() in self.negWords):
                count -=1
                print(word + " is a sad word...", end='')
        print("\n(3) Sentiment score: " + str(count) + " ", end='')

        # TODO
        return count
