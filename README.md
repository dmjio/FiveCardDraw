TO BUILD THE SOLUTION:
====================
### "make"
and then run
### "./main"

###Preface
You should see a few warnings to do with the unicode characters I am using. This is fine.
I am using unicode characters to display suite values. As far as I know this is a portable solution. I have tested it on OSX and ubuntu and it works. It only generates warnings, it does not generate errors, or inhibit the flow of the game.

###Game
In this version of 5 card draw poker I have created 4 players.
It is possible that all players be computers that play against one another.
To enable this change "#define ONLYCPU 0" to be "#define ONLYCPU 1" in the player.h file. I recommed running the game with the ONLYCPU flag marked as 0 first.

The game works by initializing all structures, building the deck, players, game, shuffling the deck, and then dealing out cards to players.

Once cards are dealt initially they can be seen by the human player. (This is kind of an unfair advantage that the human player has, but for the sake of the user I want him/her to see exactly what is going on).

After cards are dealt and displayed on the screen each user will "ante up". This means each user HAS to put 5 chips (out of their initial 100) into the pot. If a user (Human or AI) cannot put in 5 chips they automatically lose and their remaining chips will go into the pot.

A better is chosen to start out an initial round of betting. This user is chosen randomly but he must be active and he cannot have already folded his/her hand. The bet amount that this user chooses is also chosen randomly by picking a random number less than the current players chip amount. If other CPU players have more chips (or exactly the same amount of chips) that are being bet they will automatically call the bet. If a CPU player has less chips than the bet they will automatically fold. This means that a player with a high amount of chips can potentially "buy the pot." This assumption is correct, but since a betting user is chosen at random the likely hood is decreased.

After the better bets, the human user (being more intelligent) can either fold or call the bet at will. (BUT, if an AI player bets more than the chip amount of the human player the human player will automatically fold). If he chooses to call the bet a monte carlo simulation begins.

###Monte Carlo
The way I have implemented it is by using a linear scale to value every single hand.
There are 32 possible combinations of hands to exchange. For each combination I create a new deck, shuffle it, make an exchange, and add up the running total of the current value of the exchanged hand to a spot on an array of length 32. Once this has happened 20000 times for that hand. The average is calculated by dividing all the sum of all the hands by 20000. This value is stored in an array position that corresponds to one exchange combination. This process is repeated until all 32 combinations have been made.

The final step is to do a linear search for the maximum value of this array.
Once that value has been found the recommended cards to exchange are found by referencing a bit mask array that corresponds to the possible combinations. It finds all instances where the slot in the bitmask array is of value 1 and recommends those cards be exchanged.

Side note: The number of exchanges or "samples" can be changed in the montecarlo.h "#define NUMBER_OF_SAMPLES 20000" file, Right now 32 * 20,000 or 640,000 exchanges are made, note moving it up to 1,000,000 or 32,000,000 total takes at least 2 minutes each exchange. So beware.

Once this process is complete the MC advisor will ask the player if he wants to accept his recommendation. If yes, the exchange is made, if not the user can select (using a comma delimited list of the indexes of the hand array, i.e. (1,2,3) or (2,4,3) to decide what cards to exchange.

The CPU players will always accept the MC advisors recommendation.

Once all exchanges have been made everyone's new hands are displayed along with their corresponding hand values. At this point another round of betting ensues before a winner is determined. After a winner is determined all the chips in the pot go into that users chip amount property. IF 2+ users tie, meaning that they both have the same value hand (i.e.) two aces. Then the pot is split. If the split is clean (i.e. pot = 20 and 2 people tie) then both users get an equal amount and the pot goes to 0. If the split is NOT clean (i.e. pot = 23 and two players tie) then the remainder (23 mod 2 = 1) is subtracted from the pot, both players receive 11 chips and then that final coin is placed back into the pot for the next round of betting.


###Enjoy!
