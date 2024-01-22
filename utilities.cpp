//
// Created by layan on 5/19/2023.
//

#include "utilities.h"

/*********************************************** GenreStats Implementation **********************************************/

int GenreStats::getDramaStats() const
{
    return drama_stats;
}

int GenreStats::getActionStats() const
{
    return action_stats;
}

int GenreStats::getFantasyStats() const
{
    return fantasy_stats;
}

int GenreStats::getComedyStats() const
{
    return comedy_stats;
}

void GenreStats::incDramaStats(int val)
{
    drama_stats+=val;
}

void GenreStats::incActionStats(int val)
{
    action_stats+=val;
}

void GenreStats::incFantasyStats(int val)
{
    fantasy_stats+=val;
}

void GenreStats::incComedyStats(int val)
{
    comedy_stats+=val;
}

void GenreStats::resetStats()
{
    fantasy_stats = 0;
    drama_stats = 0;
    comedy_stats = 0;
    action_stats = 0;
}

//GenreStats& GenreStats::operator+=(std::shared_ptr<GenreStats> other_stats)
//{
//    fantasy_stats += other_stats->getFantasyStats();
//    drama_stats += other_stats->getDramaStats();
//    comedy_stats += other_stats->getComedyStats();
//    action_stats += other_stats->getActionStats();
//    return (*this);
//}

void GenreStats::mergeGenreStats(std::shared_ptr<GenreStats> other_stats, bool sign)
{
    if(sign)
    {
        fantasy_stats += other_stats->getFantasyStats();
        drama_stats += other_stats->getDramaStats();
        comedy_stats += other_stats->getComedyStats();
        action_stats += other_stats->getActionStats();
    }
    else
    {
        fantasy_stats -= other_stats->getFantasyStats();
        drama_stats -= other_stats->getDramaStats();
        comedy_stats -= other_stats->getComedyStats();
        action_stats -= other_stats->getActionStats();
    }
}

int GenreStats::getSumStats()
{
    return drama_stats + comedy_stats + action_stats + fantasy_stats;
}

/*********************************************** MovieKey Implementation **********************************************/

double MovieKey::getAvgRating() const
{
    return avg_rating;
}

int MovieKey::getViews() const
{
    return views;
}

int MovieKey::getMovieId() const
{
    return movieId;
}

//movieId azghar = key akbar
bool MovieKey::operator<(const MovieKey& other)
{
    if(avg_rating < other.getAvgRating()) return true;
    if(avg_rating > other.getAvgRating()) return false;
    //avg_rating is equal
    if(views < other.getViews()) return true;
    if(views > other.getViews()) return false;
    //views are equal
    if(movieId > other.getMovieId()) return true;
    return false;
}

bool MovieKey::operator==(const MovieKey& other)
{
    if(avg_rating == other.getAvgRating() && views == other.getViews() && movieId == other.getMovieId())
    {
        return true;
    }
    return false;
}

bool MovieKey::operator>(const MovieKey& other)
{
    if(this->operator<(other) || this->operator==(other))
    {
        return false;
    }
    return true;
}


/////////////////////////////////////////////// AVL helper functions: ///////////////////////////////////////////////



