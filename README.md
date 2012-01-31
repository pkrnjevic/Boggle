
Boggle
======

Last week I was asked how I might implement a Boggle solver. After a bit
of thought, I hacked a version in Ruby, then ported it to Windows using
C++ and WTL. The zip includes source for the C++ and Ruby versions, as
well as a prebuilt Windows binary and a substantial word list from YAWL.
Launch from the folder containing the word.list.

By the way, neither implementation handles 'Q' yet.

Windows Prebuilt
----------------

If you'd just like to try the prebuilt C++ version, it is included in 
**boggle.zip**, not under the prebuilt folder. It's rough and incomplete
but fun all the same.

How CDict works
---------------

One kind fellow asked, so...

First, CDict::Load reads the specified wordlist file (I use YAWL), discarding words
less than MinWordLength (default is 4 based on some rule I saw) characters
long. Also, "QU" is converted to "Q" which messes up words having "Q" not 
followed by "U", such as QAT, but such is life ... I told you it wasn't 
complete - feel free to fix it.

CDict is simply an array pointing other CDict arrays, each AlphabetSize
(26 in this case) entries long. The top level array contains entries for
first letter of each word in the dictionary, this entry points to a
second level CDict array for the next letter in the word. For example,
inserting "APPLE" into CDict would create a top level array with an
entry at index 0, "A" being the first letter in the alphabet, pointing
to a second level array with an entry at index 15 (P - A is 15) pointing
to a third level array and so on. Finally, an end-of-word marker is
placed the last level.

Once the CDict is built, finding a valid word is almost as easy as:
dict['A']['P']['P']['L']['E'] == end-of-word.

The other interesting piece is CBoard::Walk that recursively walks the
board testing letters and their neighbours against the CDict.

Other versions
--------------

I also hacked up an OCaml version that I misplaced - if anyone is
interested, I'll dig around and post it too.

License
-------

You're free to use this in any way you wish, though I would
appreciate acknowledgement. And, of course I would love to see
improvements. Perhaps someone would like to complete the game, though
the name may need to change as Boggle is a trademark of Parker Brothers.

Enjoy!

Peter


