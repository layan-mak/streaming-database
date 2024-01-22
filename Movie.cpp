//
// Created by layan on 5/19/2023.
//
#include "Movie.h"

int Movie::getId()
{
    return movieId;
}

bool Movie::isVip()
{
    return vip;
}

int Movie::getViews()
{
    return views;
}

void Movie::incViews(int newViews)
{
    views+=newViews;
}

Genre Movie::getGenre()
{
    return genre;
}

int Movie::getRating()
{
    return rating;
}

void Movie::incRating(int newRating)
{
    rating+= newRating;
}

int Movie::getNumRaters()
{
    return num_raters;
}

void Movie::incNumRaters()
{
    num_raters++;
}

double Movie::calcAvgRating()
{
    if(num_raters == 0) return 0;
    return ((double)rating / (double)num_raters);
}

MovieKey Movie::getMovieKey()
{
    double avg_rating = this->calcAvgRating();
    int views = this->getViews();
    int movieId = this->getId();
    MovieKey movieKey(avg_rating,views,movieId);
    return movieKey;
}