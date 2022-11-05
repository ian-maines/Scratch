// wordle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <map>
#include <set>

namespace
{
    template <typename T>
    bool exists (const std::set<T>& s, T v)
    {
        return s.find (v) != s.end ();
    }
}

int main()
{
    std::cout << "Loading words\n" << std::endl;
    std::ifstream f;
    f.open ("words.txt", std::ios_base::in);

    if (!f.is_open ())
          {
        std::cout << "failed to open file!";
        std::cin;
        return -1;
        }

    std::vector<std::string> words;
    std::vector<char> buf (16, 0);

    while (!(f.getline (&buf[0], 16)).eof ())
        {
        words.push_back (std::string (&buf[0]));
        }

    std::cout << "word list size = " << words.size () << std::endl;

    // Let's get some letter stats
    std::map<char, size_t> letter_stats;

    for (const auto& w : words)
    {
    for (const char c : w)
        {
        auto itr = letter_stats.find (c);
        if (itr == letter_stats.end())
            {
            letter_stats.insert (std::make_pair (c, 1));
            }
        else
            {
            itr->second++;
            }
        }
    }

    std::cout << "letter stats for " << letter_stats.size ()  << "letters" << std::endl;

    for (const auto& item : letter_stats)
        {
        std::cout << (item.first) << ":" << item.second << std::endl;
        }


// Now for each of the words, give it a ranking based on the letter stats
    struct ws_t
        {
        std::string word;
        size_t score = 0;
        };

    std::vector<ws_t> word_rankings;

    for (const auto& w : words)
        {
        ws_t ws;
        ws.word = w;
        for (const char& c : w)
            {
            ws.score += letter_stats.find (c)->second;
            }
        word_rankings.push_back (ws);
        }

    std::cout << "number of ranked words: " << word_rankings.size () << std::endl;
    std::vector<std::string> top_6;

    // Now pick the 5 highest-scoring words that cover 25 letters
    std::sort (word_rankings.begin (), word_rankings.end (), [](const ws_t& ws_l, const ws_t& ws_r) { return ws_l.score > ws_r.score; });

    const std::set<char> vowels = { 'a', 'e', 'i', 'o', 'u' };

    std::set<char> used_letters;
    for (int i = 0; i < 6; ++i)
        {
        for (const auto& w : word_rankings)
            {
            std::set<char> dupe_in_word;
            size_t letter_used = 0;
            size_t vowel_ct = 0;
            bool dupe_letter = false;

            // Check each letter in the word - have we already used it in a previous word? Does it appear twice in this word?
            for (const auto& l : w.word)
                {
                if (exists (vowels, l))
                    {
                    vowel_ct++;
                    }
                // Does this letter already exist in the word?
                if (dupe_in_word.find (l) != dupe_in_word.end ())
                    {
                    dupe_letter = true;
                    break;
                    }
                // Nope - add it.
                dupe_in_word.insert (l);

                // The goal of the decisions below is to use all vowels within the first 3 words without duplicating any vowels or consonats.
                // Once we have all of the vowels accounted for, we'll need to use them for words 4-6 to explore unused consonants
                if ((exists (vowels, l) && top_6.size () > 3) || (used_letters.find (l) != used_letters.end ()))
                    {
                    letter_used++;
                    }
                }
            if (letter_used <= 1 && vowel_ct <= 2 && !dupe_letter)
                {
                top_6.push_back (w.word);
                for (const auto& l : w.word)
                    {
                    used_letters.insert (l);
                    }
                }
            }
        }
    
        std::cout << top_6.size () << "words found" << std::endl;
        for (auto& w : top_6)
            {
            std::cout << w << " ";
            }
    


}