//
// Created by layan on 5/19/2023.
//

#ifndef AVLTREE_H_MOVIE_H
#define AVLTREE_H_MOVIE_H

#include "utilities.h"

class Movie
{
    int movieId;
    bool vip;
    int views;
    Genre genre;
    int rating;
    int num_raters;

public:
    Movie(int movieId,bool isVip,int views,Genre genre) : movieId(movieId),
    vip(isVip),views(views),genre(genre),rating(0),num_raters(0){};

    Movie(Movie& movie) = default;
    Movie& operator=(const Movie& movie) = default;
    ~Movie() = default;

    int getId();
    bool isVip();
    int getViews();
    void incViews(int views);
    Genre getGenre();
    int getRating();
    void incRating(int newRating);
    int getNumRaters();
    void incNumRaters();

    double calcAvgRating();

    MovieKey getMovieKey();
};

#endif //AVLTREE_H_MOVIE_H
