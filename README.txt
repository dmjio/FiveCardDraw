Homework 4
Poker Game
due: Dec. 6
1
You will write a 5-card draw poker application in C using 3 simulated players and
one "live" player (the user). 5-card draw is an extremely simple game -- if you aren't familiar with
it please see the wikipedia page en.wikipedia.org/woki/Five-card_draw and
en.wikipedia.org/wiki/Poker_hands. The point of the
exercise is to put together a number of concepts in the course in the context of
a semi-realistic application. It is not expected that you will have to spend much time learning the rules of the game.

The game logic is as follows:
 - create a deck of cards
 - shuffle the deck
 - deal five cards to each of the four players
 - loop over each of the three automated players and have them make a decision on which
   cards (if any) to exchange. 
    * to do this requires an AI decision making process. We will do this with a Monte
      Carlo simulation of all possible hands, resulting in a ranking of each hand and
      thus a decision on the probable best move. A separate short writeup describing this
      process will appear shortly in the dropbox. (it is easier than it sounds).
 - request input from live player (MC simulator will advise live player as well). MC advisor
   should advise on the absolute quality of the best move. See dropbox for more details
   on how to rank hands based on an absolute scale.
 - compare the hands and determine a winner

You will not receive full credit for hacking the application. Careful use of abstraction
and data structures is required. Specifically, we require the following:
	a) Build  Card, Deck, Hand, and Player abstractions.

        b) The Deck abstraction should implement the queue interface (i.e. be a queue).
       Cards dealt to players are dequeued, and for the purpose of the exercise they
	  are enqueued when they return to the deck. Note that there are multiple acceptable
	  ways of implementing the queue data structure.

	c) Build functions with clear interfaces to initialize, shuffle, deal,
      display, and exchange cards with the Deck.

	e) Build a clear interface to the monte carlo advisor so that the
      code is easy to test with e.g. less sophisticated decision making.

Grading metrics:

50% - functioning game logic with random advisor

20% - judicious use of data structures, clean function interfaces, and general style

30% - correct implementation of Monte Carlo advisor


extra credit: wrap the single hand simulator in a game loop which allows for a round of betting
at each hand, eliminates players when they run out of money, allows a player to fold, etc.
