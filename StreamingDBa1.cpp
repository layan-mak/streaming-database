#include "StreamingDBa1.h"

streaming_database::streaming_database()
{
	movies_by_id = std::make_shared<AVLTree<int, std::shared_ptr<Movie>>>();
    users_by_id = std::make_shared<AVLTree<int, std::shared_ptr<User>>>();
    groups_by_id = std::make_shared<AVLTree<int, std::shared_ptr<Group>>>();

    comedy_movies_by_ranking = std::make_shared<AVLTree<MovieKey, std::shared_ptr<Movie>>>();
    drama_movies_by_ranking = std::make_shared<AVLTree<MovieKey, std::shared_ptr<Movie>>>();
    action_movies_by_ranking = std::make_shared<AVLTree<MovieKey, std::shared_ptr<Movie>>>();
    fantasy_movies_by_ranking = std::make_shared<AVLTree<MovieKey, std::shared_ptr<Movie>>>();

    system_genre_stats = std::make_shared<GenreStats>();
}


void removeAllUsersAux1(std::shared_ptr<Node<int, std::shared_ptr<User>>> user_node)
{
    if(!user_node || !user_node->getValue()) return;
    std::shared_ptr<Node<int, std::shared_ptr<User>>> left_son = user_node->getLeftSon();
    std::shared_ptr<Node<int, std::shared_ptr<User>>> right_son = user_node->getRightSon();
    removeAllUsersAux1(left_son);
    std::shared_ptr<User> tmp = nullptr;
    user_node->setValue(tmp);
    removeAllUsersAux1(right_son);
}

void removeAllMoviesAux1(std::shared_ptr<Node<int, std::shared_ptr<Movie>>> movie_node)
{
    if(!movie_node || !movie_node->getValue()) return;
    std::shared_ptr<Node<int, std::shared_ptr<Movie>>> left_son = movie_node->getLeftSon();
    std::shared_ptr<Node<int, std::shared_ptr<Movie>>> right_son = movie_node->getRightSon();
    removeAllMoviesAux1(left_son);
    std::shared_ptr<Movie> tmp = nullptr;
    movie_node->setValue(tmp);
    removeAllMoviesAux1(right_son);
}

void removeAllGroupsAux1(std::shared_ptr<Node<int, std::shared_ptr<Group>>> group_node)
{
    if(!group_node) return;
    std::shared_ptr<Node<int, std::shared_ptr<Group>>> left_son = group_node->getLeftSon();
    std::shared_ptr<Node<int, std::shared_ptr<Group>>> right_son = group_node->getRightSon();
    removeAllGroupsAux1(left_son);
    std::shared_ptr<Group> tmp = nullptr;
    group_node->setValue(tmp);
    removeAllGroupsAux1(right_son);
}

void removeAllMoviesByGenreAux1(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>> movie_node)
{
    if(!movie_node) return;
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>> left_son = movie_node->getLeftSon();
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>> right_son = movie_node->getRightSon();
    removeAllMoviesByGenreAux1(left_son);
    std::shared_ptr<Movie> tmp = nullptr;
    movie_node->setValue(tmp);
    removeAllMoviesByGenreAux1(right_son);
}

void removeGroupMembersTree(std::shared_ptr<Node<int, std::shared_ptr<Group>>> group_node)
{
    if(!group_node) return;
    std::shared_ptr<Node<int, std::shared_ptr<Group>>> left_son = group_node->getLeftSon();
    std::shared_ptr<Node<int, std::shared_ptr<Group>>> right_son = group_node->getRightSon();
    removeGroupMembersTree(left_son);
    if(group_node->getValue() && group_node->getValue()->getUsersTree())
    {
        removeAllUsersAux1(group_node->getValue()->getUsersTree()->getRoot());
    }
    removeGroupMembersTree(right_son);
}

streaming_database::~streaming_database()
{
    //removeGroupMembersTree(groups_by_id->getRoot());
    removeAllGroupsAux1(groups_by_id->getRoot());
    removeAllUsersAux1(users_by_id->getRoot());
    removeAllMoviesAux1(movies_by_id->getRoot());
    removeAllMoviesByGenreAux1(comedy_movies_by_ranking->getRoot());
    removeAllMoviesByGenreAux1(action_movies_by_ranking->getRoot());
    removeAllMoviesByGenreAux1(drama_movies_by_ranking->getRoot());
    removeAllMoviesByGenreAux1(fantasy_movies_by_ranking->getRoot());
    groups_by_id->destroyTreeAux(groups_by_id->getRoot());
    users_by_id->destroyTreeAux(users_by_id->getRoot());
    movies_by_id->destroyTreeAux(movies_by_id->getRoot());
    comedy_movies_by_ranking->destroyTreeAux(comedy_movies_by_ranking->getRoot());
    action_movies_by_ranking->destroyTreeAux(action_movies_by_ranking->getRoot());
    drama_movies_by_ranking->destroyTreeAux(drama_movies_by_ranking->getRoot());
    fantasy_movies_by_ranking->destroyTreeAux(fantasy_movies_by_ranking->getRoot());
}

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if(movieId <= 0 || genre == Genre::NONE || views < 0)
    {
        return StatusType::INVALID_INPUT;
    }

    if(movies_by_id->findNode(movieId) != nullptr) //movie already exists
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Movie> movie_ptr;
    try
    {
        movie_ptr = std::make_shared<Movie>(movieId, vipOnly, views, genre);
    }
    //std::shared_ptr<Movie> movie_ptr = std::make_shared<Movie>(movieId, vipOnly, views, genre);
    catch (std::bad_alloc)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    movies_by_id->insertNode(movieId, movie_ptr);
    MovieKey movie_key = movie_ptr->getMovieKey();

    switch(genre)
    {
        case Genre::DRAMA:
            drama_movies_by_ranking->insertNode(movie_key, movie_ptr);
            system_genre_stats->incDramaStats();
            break;
        case Genre::FANTASY:
            fantasy_movies_by_ranking->insertNode(movie_key, movie_ptr);
            system_genre_stats->incFantasyStats();
            break;
        case Genre::COMEDY:
            comedy_movies_by_ranking->insertNode(movie_key, movie_ptr);
            system_genre_stats->incComedyStats();
            break;
        case Genre::ACTION:
            action_movies_by_ranking->insertNode(movie_key, movie_ptr);
            system_genre_stats->incActionStats();
            break;
        case Genre::NONE:
            break;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
	if(movieId <= 0){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Node<int,std::shared_ptr<Movie>>> movie_node=movies_by_id->findNode(movieId);
    if(!movie_node)
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Movie> movie = movie_node->getValue();
    MovieKey movie_key = movie->getMovieKey();
    Genre genre = movie->getGenre();

    switch (genre) {
        case Genre::COMEDY:
            comedy_movies_by_ranking->removeNode(movie_key);
            system_genre_stats->incComedyStats(-1);
            break;
        case Genre::ACTION:
            action_movies_by_ranking->removeNode(movie_key);
            system_genre_stats->incActionStats(-1);
            break;
        case Genre::FANTASY:
            fantasy_movies_by_ranking->removeNode(movie_key);
            system_genre_stats->incFantasyStats(-1);
            break;
        case Genre::DRAMA:
            drama_movies_by_ranking->removeNode(movie_key);
            system_genre_stats->incDramaStats(-1);
            break;
        case Genre::NONE:
            break;
    }
    movies_by_id->removeNode(movieId);
    
	return StatusType::SUCCESS;
}

//should use std::make_shared to allocate new std::shared_ptr for User node VALUE
StatusType streaming_database::add_user(int userId, bool isVip)
{
    if(userId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if(users_by_id->findNode(userId))
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<User> new_user;
    try
    {
        new_user = std::make_shared<User>(userId,isVip);
    } catch (std::bad_alloc)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    bool res = users_by_id->insertNode(userId, new_user);
    if(!res)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
//    if(!users_by_id->insertNode(userId,new_user))
//    {
//        return StatusType::FAILURE;
//    }
//	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if(userId <= 0){
        return StatusType::INVALID_INPUT;
    }
	std::shared_ptr<Node<int,std::shared_ptr<User>>> to_remove = users_by_id->findNode(userId);
    if(to_remove == nullptr)
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<User> user = to_remove->getValue();
    if(user->isInGroup())
    {
        std::weak_ptr<Group> user_group = user->getUserGroup();
        std::shared_ptr<Group> user_group_shared = user_group.lock();
        if(user->isVip())
        {
            user_group_shared->decNumVipUsersInGroup();
        }
        user_group_shared->decNumUsersInGroup();
        std::shared_ptr<AVLTree<int,std::shared_ptr<User>>> group_users_tree = user_group_shared->getUsersTree();
        group_users_tree->removeNode(userId);
        //std::shared_ptr<GenreStats> user_real_views = user->calcActualUserStats();

        std::shared_ptr<GenreStats> exclusive_when_joined = user->getExclusiveWhenJoined();
        //user_group_shared->updateSumUserViews(user_real_views, false);
        user_group_shared->updateSumUserViews(exclusive_when_joined, false);
        user_group_shared->updateSumUserViews(user->getUserGenreViews(), false);

        std::shared_ptr<GenreStats> exc_group_views = user_group_shared->getExclusiveGroupViews();
        std::shared_ptr<GenreStats> user_genre_views = user->getUserGenreViews();
        std::shared_ptr<GenreStats> diff;
        try
        {
            diff = std::make_shared<GenreStats>(*exc_group_views);
        }
        catch (std::bad_alloc)
        {
            user_group_shared->incNumUsersInGroup();
            if(user->isVip()) user_group_shared->incNumVipUsersInGroup();
            //user_group_shared->updateSumUserViews(user_real_views, true);
            user_group_shared->updateSumUserViews(exclusive_when_joined, true);
            user_group_shared->updateSumUserViews(user->getUserGenreViews(), true);
            user_group_shared->getUsersTree()->insertNode(userId, user);
            return StatusType::ALLOCATION_ERROR;
        }
        diff->mergeGenreStats(exclusive_when_joined, false);
        std::shared_ptr<GenreStats> exc_multiplied_stats = user_group_shared->getMultipliedExclusiveGroupViews();
        exc_multiplied_stats->mergeGenreStats(diff, false);
        diff = nullptr;
        user->setInGroup(false);
        if(user_group_shared->getNumUsersInGroup() == 0)
        {
            user_group_shared->getSumUserViews()->resetStats();
            user_group_shared->getExclusiveGroupViews()->resetStats();
            user_group_shared->getMultipliedExclusiveGroupViews()->resetStats();
        }
        exclusive_when_joined->resetStats();
    }
    users_by_id->removeNode(userId);
	return StatusType::SUCCESS;
}

//should use std::make_shared to allocate new std::shared_ptr for Group node VALUE
StatusType streaming_database::add_group(int groupId)
{
	if(groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if(groups_by_id->findNode(groupId))
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Group> new_group;
    try
    {
        new_group = std::make_shared<Group>(groupId);
    }
    catch (std::bad_alloc)
    {
        //new_group = nullptr;
        return StatusType::ALLOCATION_ERROR;
    }
    bool res = groups_by_id->insertNode(groupId, new_group);
    if(!res)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
	if(groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Node<int, std::shared_ptr<Group>>> group_node = groups_by_id->findNode(groupId);
    if(group_node == nullptr) //group doesn't exist
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Group> group = group_node->getValue();
    group->updateUserViews();
    group->removeAllUsers(); //sets each user's inGroup to false
    groups_by_id->removeNode(groupId);
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
	if(userId <= 0 || groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Node<int, std::shared_ptr<User>>> user_node = users_by_id->findNode(userId);
    std::shared_ptr<Node<int, std::shared_ptr<Group>>> group_node = groups_by_id->findNode(groupId);
    if(!user_node || !group_node || !user_node->getValue() || !group_node->getValue()
                                                || user_node->getValue()->isInGroup())
        //last condition is to check if user is already in another group
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Group> group = group_node->getValue();
    std::shared_ptr<User> user = user_node->getValue();

    //add the user to the group and update group info:
    user->setUserGroup(group);
    user->setInGroup(true);
    user->getExclusiveWhenJoined()->resetStats();
    user->getExclusiveWhenJoined()->mergeGenreStats(group->getExclusiveGroupViews() ,true); //new
    group->getUsersTree()->insertNode(userId, user); //assumes user_tree isn't null
    group->incNumUsersInGroup();
    if(user->isVip())
    {
        group->incNumVipUsersInGroup();
    }
    group->updateSumUserViews(user->getUserGenreViews(), true);          //sum_users_views += user_genre_views
    user->updateUserGenreViews(group->getExclusiveGroupViews(), false); //user_genre_views -= exclusive_group_views

    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
	if(userId <= 0 || movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Node<int, std::shared_ptr<User>>> user_node = users_by_id->findNode(userId);
    std::shared_ptr<Node<int, std::shared_ptr<Movie>>> movie_node = movies_by_id->findNode(movieId);

    if(!user_node || !user_node->getValue() || !movie_node || !movie_node->getValue())
    {
        return StatusType::FAILURE;
    }

    std::shared_ptr<User> user = user_node->getValue();
    std::shared_ptr<Movie> movie = movie_node->getValue();

    if(movie->isVip() && !user->isVip())
    {
        return StatusType::FAILURE;
    }

    //save old key to genre_movies_by_ranking for this movie:
    MovieKey old_key = movie->getMovieKey();

    //update movie, user, and the group the user belongs to 's stats:

    Genre genre = movie->getGenre();
    movie->incViews(1);
    user->incUserGenreViews(genre);
    std::weak_ptr<Group> user_group = user->getUserGroup();
    if(user->isInGroup())
    {
        user_group.lock()->incSumUserViews(genre);
    }

    //update genre_movies_by_ranking tree:
    MovieKey new_key = movie->getMovieKey();

    switch(genre)
    {
        case Genre::DRAMA:
            drama_movies_by_ranking->removeNode(old_key);
            drama_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::ACTION:
            action_movies_by_ranking->removeNode(old_key);
            action_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::COMEDY:
            comedy_movies_by_ranking->removeNode(old_key);
            comedy_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::FANTASY:
            fantasy_movies_by_ranking->removeNode(old_key);
            fantasy_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::NONE:
            break;
    }

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
	if(groupId <= 0 || movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Node<int, std::shared_ptr<Group>>> group_node = groups_by_id->findNode(groupId);
    std::shared_ptr<Node<int, std::shared_ptr<Movie>>> movie_node = movies_by_id->findNode(movieId);

    if(!group_node || !group_node->getValue() || !movie_node || !movie_node->getValue())
    {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Group> group = group_node->getValue();
    std::shared_ptr<Movie> movie = movie_node->getValue();

    if((movie->isVip() && !group->isVip()) || group->getNumUsersInGroup() == 0)
    {
        return StatusType::FAILURE;
    }
    //save old key to genre_movies_by_ranking for this movie:
    MovieKey old_key = movie->getMovieKey();

    Genre genre = movie->getGenre();
    movie->incViews(group->getNumUsersInGroup());
    group->incExclusiveGroupViews(genre);
    group->updateMultipliedExclusiveGroupViews(genre);

    MovieKey new_key = movie->getMovieKey();

    switch(genre) {
        case Genre::DRAMA:
            drama_movies_by_ranking->removeNode(old_key);
            drama_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::ACTION:
            action_movies_by_ranking->removeNode(old_key);
            action_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::COMEDY:
            comedy_movies_by_ranking->removeNode(old_key);
            comedy_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::FANTASY:
            fantasy_movies_by_ranking->removeNode(old_key);
            fantasy_movies_by_ranking->insertNode(new_key, movie);
            break;
        case Genre::NONE:
            break;
    }
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre) {
    int count = 0;
    if (genre == Genre::NONE) {
        count += system_genre_stats->getDramaStats() + system_genre_stats->getActionStats()
                 + system_genre_stats->getComedyStats() + system_genre_stats->getFantasyStats();
    } else {
        switch (genre) {
            case Genre::DRAMA:
                count += system_genre_stats->getDramaStats();
                break;
            case Genre::COMEDY:
                count += system_genre_stats->getComedyStats();
                break;
            case Genre::ACTION:
                count += system_genre_stats->getActionStats();
                break;
            case Genre::FANTASY:
                count += system_genre_stats->getFantasyStats();
                break;
            case Genre::NONE:
                break;
        }
    }

    output_t<int> ret_val(count);
    return ret_val;
}

void printArr(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr, int size)
{
    for(int i = 0; i<size ; i++)
    {
        std::cout << "arr[" << i << "]: " << arr[i]->getKey().getMovieId()  << std::endl;
    }
}

StatusType getAllMoviesAux(Genre genre, std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> comedy_tree,
                     std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> drama_tree,
                     std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> action_tree,
                     std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> fantasy_tree,
                     int* const output)
{
    int comedy_size = comedy_tree->getTreeSize();
    int drama_size = drama_tree->getTreeSize();
    int action_size = action_tree->getTreeSize();
    int fantasy_size = fantasy_tree->getTreeSize();

    int new_size = comedy_size + drama_size + action_size + fantasy_size;

    //create 4 arrays for each movie genre
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* comedy_arr =
            new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[comedy_size];
    if(!comedy_arr) return StatusType::ALLOCATION_ERROR;
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* drama_arr =
            new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[drama_size];
    if(!drama_arr)
    {
        delete[] comedy_arr;
        return StatusType::ALLOCATION_ERROR;
    }
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* action_arr =
            new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[action_size];
    if(!action_arr)
    {
        delete[] comedy_arr;
        delete[] drama_arr;
        return StatusType::ALLOCATION_ERROR;
    }
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* fantasy_arr =
            new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[fantasy_size];
    if(!fantasy_arr)
    {
        delete[] action_arr;
        delete[] comedy_arr;
        delete[] drama_arr;
        return StatusType::ALLOCATION_ERROR;
    }

    if(genre == Genre::NONE)
    {
        //move each movie genre tree to array, sorted from small to big by movieKey
        moveToArray(comedy_tree, comedy_arr, comedy_size);
        moveToArray(drama_tree, drama_arr, drama_size);
        moveToArray(action_tree, action_arr, action_size);
        moveToArray(fantasy_tree, fantasy_arr, fantasy_size);

        //merge all 4 arrays to 1 sorted array by movieKey (in increasing order)
        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* tmp_arr1 =
                new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[comedy_size + drama_size];
        if(!tmp_arr1)
        {
            delete[] action_arr;
            delete[] comedy_arr;
            delete[] drama_arr;
            delete[] fantasy_arr;
            return StatusType::ALLOCATION_ERROR;
        }
        mergeTwoArrays(comedy_arr, comedy_size, drama_arr, drama_size, tmp_arr1);
        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* tmp_arr2 =
                new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[action_size + fantasy_size];
        if(!tmp_arr2)
        {
            delete[] action_arr;
            delete[] comedy_arr;
            delete[] drama_arr;
            delete[] fantasy_arr;
            delete[] tmp_arr1;
            return StatusType::ALLOCATION_ERROR;
        }
        mergeTwoArrays(action_arr, action_size, fantasy_arr, fantasy_size, tmp_arr2);

        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* movie_nodes_arr = new std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>[new_size];
        if(!movie_nodes_arr)
        {
            delete[] action_arr;
            delete[] comedy_arr;
            delete[] drama_arr;
            delete[] fantasy_arr;
            delete[] tmp_arr1;
            delete[] tmp_arr2;
            return StatusType::ALLOCATION_ERROR;
        }
        mergeTwoArrays(tmp_arr1, comedy_size+drama_size, tmp_arr2, action_size+fantasy_size, movie_nodes_arr);

        //copy the merged array to another movieId array by DECREASING order
        //movie_ids_arr = new int[new_size];
        moveNodeArrToIntArr(movie_nodes_arr, output, new_size);

        delete[] tmp_arr1;
        delete[] tmp_arr2;
        delete[] movie_nodes_arr;
    }
    else
    {
        switch (genre)
        {
            case Genre::COMEDY:
                moveToArray(comedy_tree, comedy_arr, comedy_size);
                moveNodeArrToIntArr(comedy_arr, output, comedy_size);
                break;
            case Genre::DRAMA:
                moveToArray(drama_tree, drama_arr, drama_size);
                moveNodeArrToIntArr(drama_arr, output, drama_size);
                break;
            case Genre::ACTION:
                moveToArray(action_tree, action_arr, action_size);
                moveNodeArrToIntArr(action_arr, output, action_size);
                break;
            case Genre::FANTASY:
                moveToArray(fantasy_tree, fantasy_arr, fantasy_size);
                moveNodeArrToIntArr(fantasy_arr, output, fantasy_size);
                break;
            case Genre::NONE:
                break;
        }
    }
    delete[] comedy_arr;
    delete[] drama_arr;
    delete[] action_arr;
    delete[] fantasy_arr;
    return StatusType::SUCCESS;
}


StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if(output == NULL)
    {
        return StatusType::INVALID_INPUT;
    }
    if(system_genre_stats->getSumStats() <= 0)
    {
        return StatusType::FAILURE;
    }

    if(((genre == Genre::COMEDY) && (comedy_movies_by_ranking->getTreeSize() <= 0)) ||
            ((genre == Genre::DRAMA) && (drama_movies_by_ranking->getTreeSize() <= 0)) ||
            ((genre == Genre::ACTION) && (action_movies_by_ranking->getTreeSize() <= 0)) ||
            ((genre == Genre::FANTASY) && (fantasy_movies_by_ranking->getTreeSize() <= 0)))
    {
        return StatusType::FAILURE;
    }

    StatusType ret_val = getAllMoviesAux(genre, comedy_movies_by_ranking, drama_movies_by_ranking,
                                         action_movies_by_ranking, fantasy_movies_by_ranking, output);
    return ret_val;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	if(userId <= 0)
    {
        output_t<int> ret_val(StatusType::INVALID_INPUT);
        return ret_val;
    }

    std::shared_ptr<Node<int, std::shared_ptr<User>>> user_node = users_by_id->findNode(userId);
    if(!user_node || !user_node->getValue())
    {
        output_t<int> ret_val(StatusType::FAILURE);
        return ret_val;
    }

    int count = 0;

    std::shared_ptr<User> user = user_node->getValue();
    std::shared_ptr<GenreStats> user_stats = user->calcActualUserStats();
    if(genre == Genre::NONE)
    {
        count += user_stats->getDramaStats() + user_stats->getFantasyStats()
                + user_stats->getActionStats() + user_stats->getComedyStats();
    }
    else
    {
        switch (genre)
        {
            case Genre::DRAMA:
                count += user_stats->getDramaStats();
                break;
            case Genre::COMEDY:
                count += user_stats->getComedyStats();
                break;
            case Genre::ACTION:
                count += user_stats->getActionStats();
                break;
            case Genre::FANTASY:
                count += user_stats->getFantasyStats();
                break;
            case Genre::NONE:
                break;
        }
    }
    output_t<int> ret_val(count);
    return ret_val;

}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if(userId <= 0 || movieId <=0)
    {
        return StatusType::INVALID_INPUT;
    }
    if(rating < 0 || rating > 100)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Node<int,std::shared_ptr<Movie>>> movie_to_rate = movies_by_id->findNode(movieId);
    std::shared_ptr<Node<int,std::shared_ptr<User>>> rater = users_by_id->findNode(userId);
    if(movie_to_rate == nullptr || rater == nullptr || !movie_to_rate->getValue() || !rater->getValue())
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<User> user = rater->getValue();
    std::shared_ptr<Movie> movie = movie_to_rate->getValue();
    if(!user->isVip() && movie->isVip())
    {
        return StatusType::FAILURE;
    }
    MovieKey old_movieKey = movie->getMovieKey();
    movie->incRating(rating);
    movie->incNumRaters();
    MovieKey new_movieKey = movie->getMovieKey();
    Genre genre = movie->getGenre();
    switch (genre)
    {
        case Genre::COMEDY:
            comedy_movies_by_ranking->removeNode(old_movieKey);
            comedy_movies_by_ranking->insertNode(new_movieKey,movie);
            break;
        case Genre::DRAMA:
            drama_movies_by_ranking->removeNode(old_movieKey);
            drama_movies_by_ranking->insertNode(new_movieKey,movie);
            break;
        case Genre::ACTION:
            action_movies_by_ranking->removeNode(old_movieKey);
            action_movies_by_ranking->insertNode(new_movieKey,movie);
            break;
        case Genre::FANTASY:
            fantasy_movies_by_ranking->removeNode(old_movieKey);
            fantasy_movies_by_ranking->insertNode(new_movieKey,movie);
            break;
        case Genre::NONE:
            break;
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
	if(groupId <= 0 )
    {
        output_t<int> val(StatusType::INVALID_INPUT);
        return val;
    }
    std::shared_ptr<Node<int,std::shared_ptr<Group>>> group_ptr = groups_by_id->findNode(groupId);
    if(group_ptr==nullptr) // no such group in system
    {
        output_t<int> val(StatusType::FAILURE);
        return val;
    }
    std::shared_ptr<Group> group = group_ptr->getValue();
    if(group->getNumUsersInGroup()==0) //group is empty
    {
        output_t<int> val(StatusType::FAILURE);
        return val;
    }
    std::shared_ptr<Movie> best_movie = group->calcBestMovie(comedy_movies_by_ranking, drama_movies_by_ranking,
                                                             action_movies_by_ranking, fantasy_movies_by_ranking);
    if(best_movie == nullptr)
    {
        output_t<int> ret_val(StatusType::FAILURE);
        return ret_val;
    }
    int movie_id = best_movie->getId();
    output_t<int> ret_val(movie_id);
    return ret_val;
}


