//
// Created by layan on 5/21/2023.
//

#ifndef GROUP_H_USERANDGROUP_H
#define GROUP_H_USERANDGROUP_H

#include "utilities.h"
#include "Movie.h"

class User;

class Group
{
    int groupId;
    int num_users_in_group;
    int num_vip_users_in_group;
    std::shared_ptr<AVLTree<int, std::shared_ptr<User>>> users_in_group;
    std::shared_ptr<GenreStats> exclusive_group_views; //number of movies the group has watched ever since the group was created
    std::shared_ptr<GenreStats> multiplied_exclusive_group_views;
    std::shared_ptr<GenreStats> sum_users_views; //sum of all users_in_group's individual watches (before & after joining the group)

public:
    Group(int groupId) : groupId(groupId), num_users_in_group(0), num_vip_users_in_group(0)
    {
        users_in_group = std::make_shared<AVLTree<int, std::shared_ptr<User>>>();
        exclusive_group_views = std::make_shared<GenreStats>();
        multiplied_exclusive_group_views = std::make_shared<GenreStats>(); //when will this be deleted?
        sum_users_views = std::make_shared<GenreStats>();
    }

    ~Group()
    {
        //updateUserViews();
        removeAllUsers();
        //users_in_group->destroyTreeAux(users_in_group->getRoot());
    }

    void setUsersTree(std::shared_ptr<AVLTree<int, std::shared_ptr<User>>> tree);

    Group(const Group& group_to_copy) = default;

    Group& operator=(const Group& group) = default;

    bool isVip();

    int getGroupId();

    int getNumUsersInGroup();

    void incNumUsersInGroup(); //increment

    void decNumUsersInGroup();

    int getNumVipUsersInGroup();

    void incNumVipUsersInGroup();

    void decNumVipUsersInGroup();

    std::shared_ptr<AVLTree<int, std::shared_ptr<User>>> getUsersTree();

    std::shared_ptr<GenreStats> getExclusiveGroupViews();

    std::shared_ptr<GenreStats> getMultipliedExclusiveGroupViews();

    void incExclusiveGroupViews(Genre genre);

    std::shared_ptr<GenreStats> getSumUserViews();

    //this function is used when a new user joins the group, the user's views are added to the "accumulated" users views in the group
    void updateSumUserViews(std::shared_ptr<GenreStats> user_genre_views, bool sign);

    //updates views' specific given genre
    //used in user_watch
    void incSumUserViews(Genre genre);

    void updateMultipliedExclusiveGroupViews(Genre genre);

    std::shared_ptr<GenreStats> calcActualGroupStats();

    Genre calcBestGenre();

    std::shared_ptr<Movie> calcBestMovie(std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> comedy_tree,
                                         std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> drama_tree,
                                         std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> action_tree,
                                         std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> fantasy_tree);

    /*This function scans users_in_group tree and sets user_genre_views for each user to be Actual user views (uses calc func)
     */
    void updateUserViews();

    /*This function scans users_in_group tree and for each user: sets the group e belongs to, to nullptr
     * NOTE: this function does not remove users from AVL tree! This should be done automatically when Group destructor calls AVL destructor.
     */

    void removeAllUsers();

    void resetGroupGenreViews();

//    void setMultiplied(std::shared_ptr<GenreStats> stats);
};

class User
{
    int userId;
    bool vip;
    //std::shared_ptr<Group> user_group; //will be initialized when user is added to group
    std::weak_ptr<Group> user_group;
    std::shared_ptr<GenreStats> user_genre_views;
    bool inGroup;
    std::shared_ptr<GenreStats> group_exclusive_views_when_joined;

public:
    User(int userId,bool isVip) : userId(userId),vip(isVip), inGroup(false)
    {
        user_genre_views = std::make_shared<GenreStats>();
        group_exclusive_views_when_joined = std::make_shared<GenreStats>();
    }
    //calling genrestats constructor to intialize struct to zeros////

    User(User& user)=default;

    User& operator=(const User& user)=default;

    ~User()=default;

    std::shared_ptr<GenreStats> getExclusiveWhenJoined();

/*
    void setExclusiveWhenJoined(std::shared_ptr<GenreStats> stats)
    {
        group_exclusive_views_when_joined->resetStats();
        group_exclusive_views_when_joined->mergeGenreStats(stats, true);
    }
*/

    //returns the field inGroup
    bool isInGroup();

    //changes the field inGroup
    void setInGroup(bool in_group);

    /*~User()
    {
        //user_group = nullptr;
        user_genre_views = nullptr;
    }*/

    std::shared_ptr<GenreStats> getUserGenreViews();

    //updates user genre views by adding/decreasing group_genre_views to it
    //decreasing or only adding?? BOTH
    void updateUserGenreViews(std::shared_ptr<GenreStats> group_genre_views, bool sign);

    //updates user genre views' specific given genre
    void incUserGenreViews(Genre genre);

    //std::shared_ptr<Group> getUserGroup();
    std::weak_ptr<Group> getUserGroup();

    //void setUserGroup(std::shared_ptr<Group> new_group);

    void setUserGroup(std::weak_ptr<Group> new_group);

    bool isVip();

    std::shared_ptr<GenreStats> calcActualUserStats();

};


#endif //GROUP_H_USERANDGROUP_H
