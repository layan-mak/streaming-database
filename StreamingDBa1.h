// 
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef STREAMINGDBA1_H_
#define STREAMINGDBA1_H_

#include "utilities.h"
#include "UserAndGroup.h"
#include "helper_funcs.h"

class streaming_database {
private:
    std::shared_ptr<AVLTree<int,std::shared_ptr<Movie>>> movies_by_id;
    std::shared_ptr<AVLTree<int,std::shared_ptr<User>>> users_by_id;
    std::shared_ptr<AVLTree<int,std::shared_ptr<Group>>> groups_by_id;

    std::shared_ptr<AVLTree<MovieKey,std::shared_ptr<Movie>>> comedy_movies_by_ranking;
    std::shared_ptr<AVLTree<MovieKey,std::shared_ptr<Movie>>> drama_movies_by_ranking;
    std::shared_ptr<AVLTree<MovieKey,std::shared_ptr<Movie>>> action_movies_by_ranking;
    std::shared_ptr<AVLTree<MovieKey,std::shared_ptr<Movie>>> fantasy_movies_by_ranking;

    std::shared_ptr<GenreStats> system_genre_stats; //number of movies by genre

public:
	// <DO-NOT-MODIFY> {
	streaming_database();

	virtual ~streaming_database();
	
	StatusType add_movie(int movieId, Genre genre, int views, bool vipOnly);
	
	StatusType remove_movie(int movieId);
	
	StatusType add_user(int userId, bool isVip);
	
	StatusType remove_user(int userId);
	
	StatusType add_group(int groupId);
	
	StatusType remove_group(int groupId);

	StatusType add_user_to_group(int userId, int groupId);
	
	StatusType user_watch(int userId, int movieId);

    StatusType group_watch(int groupId,int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);
	
	output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);
	
	output_t<int> get_group_recommendation(int groupId);
	
	// } </DO-NOT-MODIFY>
};

#endif // STREAMINGDBA1_H_
