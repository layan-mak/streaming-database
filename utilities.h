//
// Created by layan on 5/19/2023.
//

#ifndef AVLTREE_H_UTILITIES_H
#define AVLTREE_H_UTILITIES_H

#include "wet1util.h"
#include "AVLTree.h"

class GenreStats
{
    int drama_stats;
    int action_stats;
    int fantasy_stats;
    int comedy_stats;

public:
    GenreStats() = default;

    ~GenreStats() = default;

    GenreStats(const GenreStats& stats_to_copy) = default;

    GenreStats& operator=(const GenreStats& genre_stats) = default;

    int getDramaStats() const;

    int getActionStats() const;

    int getFantasyStats()const;

    int getComedyStats() const;

    void incDramaStats(int val=1);

    void incActionStats(int val=1);

    void incFantasyStats(int val=1);

    void incComedyStats(int val=1);

    //sets all stats to 0
    void resetStats();

//    GenreStats& operator+=(std::shared_ptr<GenreStats> other_stats);

    //if sign is true, other_stats are added to curr stats, otherwise decreased from them
    void mergeGenreStats(std::shared_ptr<GenreStats> other_stats, bool sign);

    int getSumStats();
};

class MovieKey
{
    double avg_rating;
    int views;
    int movieId;
    
public:
    MovieKey(double avg_rating, int views, int movieId) : avg_rating(avg_rating), views(views), movieId(movieId){};

    ~MovieKey() = default;

    MovieKey(const MovieKey& movie_key_to_copy) = default;

    MovieKey& operator=(const MovieKey& movie_key) = default;

    double getAvgRating() const;

    int getViews() const;

    int getMovieId() const;

    bool operator<(const MovieKey& other);

    bool operator>(const MovieKey& other);

    bool operator==(const MovieKey& other);
};



#endif //AVLTREE_H_UTILITIES_H
