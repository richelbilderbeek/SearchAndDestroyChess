# SearchAndDestroyChess

[![Travis CI logo](TravisCI.png)](https://travis-ci.org)

[![Build Status](https://travis-ci.org/richelbilderbeek/SearchAndDestroyChess.svg?branch=master)](https://travis-ci.org/richelbilderbeek/SearchAndDestroyChess)

Chess game with a fog of war, programmed in C++ using SFML.

The following special rules apply to SearchAndDestroyChess:

 * A player cannot be in chess, nor in checkmate. Instead, if the king is captured, the capturer wins. This is because it is possible for a king to be in check without knowing it
 * Normal rules to en passant capture apply. But when a pawn can be captured en passant, this pawn is visible. This makes a pawn look sideways for only one turn. This is because the attacker must be able to see that he/she can make an en passant capture
 * Normal rules to castling apply, except that castling can take place when in chess and through attacked squares. This is because it is possible for a king to be or getting in check without knowing it
 * A pawn always gets promoted to a queen

The first version of SearchAndDestroyChess was put online at 2008-07-06. I month later I 
found out I wasn't the first to have invented this variant: 
it is also known as [Kriegspiel](https://en.wikipedia.org/wiki/Kriegspiel_%28chess%29)
or [Dark Chess.](https://en.wikipedia.org/wiki/Dark_chess)

![SearchAndDestroyChess v3.1](Screenshots/SearchAndDestroyChess_3_1.png)

## Older screenshots

![SearchAndDestroyChess v3.0](Screenshots/SearchAndDestroyChess_3_0.png)

![SearchAndDestroyChess v3.0](Screenshots/SearchAndDestroyChess_2_0.png)

![SearchAndDestroyChess v3.0](Screenshots/SearchAndDestroyChess_1_0.png)

