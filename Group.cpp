//
// Created by layan on 5/19/2023.
//

#include "UserAndGroup.h"

int Group::getGroupId()
{
    return groupId;
}

int Group::getNumUsersInGroup()
{
    return num_users_in_group;
}

void Group::incNumUsersInGroup()
{
    num_users_in_group++;
}

void Group::decNumUsersInGroup()
{
    num_users_in_group--;
}

int Group::getNumVipUsersInGroup()
{
    return num_vip_users_in_group;
}

void Group::incNumVipUsersInGroup()
{
    num_vip_users_in_group++;
}

void Group::decNumVipUsersInGroup()
{
    num_vip_users_in_group--;
}

std::shared_ptr<GenreStats> Group::getExclusiveGroupViews()
{
    return exclusive_group_views;
}

std::shared_ptr<GenreStats> Group::getMultipliedExclusiveGroupViews()
{
    return multiplied_exclusive_group_views;
}

void Group::incExclusiveGroupViews(Genre genre)
{
    switch(genre)
    {
        case Genre::DRAMA:
            exclusive_group_views->incDramaStats();
            break;
        case Genre::ACTION:
            exclusive_group_views->incActionStats();
            break;
        case Genre::COMEDY:
            exclusive_group_views->incComedyStats();
            break;
        case Genre::FANTASY:
            exclusive_group_views->incFantasyStats();
            break;
        case Genre::NONE:
            break;
    }
}

std::shared_ptr<GenreStats> Group::getSumUserViews()
{
    return sum_users_views;
}

void Group::updateSumUserViews(std::shared_ptr<GenreStats> user_genre_views, bool sign)
{
    sum_users_views->mergeGenreStats(user_genre_views, sign);
}

void Group::updateMultipliedExclusiveGroupViews(Genre genre)
{
    switch(genre)
    {
        case Genre::DRAMA:
            multiplied_exclusive_group_views->incDramaStats(num_users_in_group);
            break;
        case Genre::ACTION:
            multiplied_exclusive_group_views->incActionStats(num_users_in_group);
            break;
        case Genre::COMEDY:
            multiplied_exclusive_group_views->incComedyStats(num_users_in_group);
            break;
        case Genre::FANTASY:
            multiplied_exclusive_group_views->incFantasyStats(num_users_in_group);
            break;
        case Genre::NONE:
            break;
    }
}

std::shared_ptr<GenreStats> Group::calcActualGroupStats()
{
    std::shared_ptr<GenreStats> actual_stats = std::make_shared<GenreStats>();
    actual_stats->mergeGenreStats(sum_users_views, true);
    actual_stats->mergeGenreStats(multiplied_exclusive_group_views, true); //dim
    return actual_stats;
}

std::shared_ptr<AVLTree<int, std::shared_ptr<User>>> Group::getUsersTree()
{
    return users_in_group;
}

bool Group::isVip()
{
    if(num_vip_users_in_group > 0) return true;
    return false;
}

//this is the inside function to use in InOrder
void updateUserViewAux(std::shared_ptr<Node<int, std::shared_ptr<User>>> user_node ,std::shared_ptr<GenreStats> exclusive_group_views, int trash1, int trash2)
{
    if(!user_node || !user_node->getValue()) return;
    std::shared_ptr<User> user = user_node->getValue();
    user->updateUserGenreViews(exclusive_group_views, true);
}

void Group::updateUserViews()
{
    users_in_group->inOrder(users_in_group->getRoot(), updateUserViewAux, exclusive_group_views, 0, 0);
}

void removeAllUsersAux(std::shared_ptr<Node<int, std::shared_ptr<User>>> user_node, int trash1, int trash2, int trash3)
{
    if(!user_node || !user_node->getValue()) return;
    //user_node->getValue()->setUserGroup(nullptr);
    user_node->getValue()->setInGroup(false);
    user_node->getValue()->getExclusiveWhenJoined()->resetStats();
}

void Group::removeAllUsers()
{
    users_in_group->inOrder(users_in_group->getRoot(), removeAllUsersAux, 0, 0, 0);
}

void Group::setUsersTree(std::shared_ptr<AVLTree<int, std::shared_ptr<User>>> tree)
{
    users_in_group = tree;
}

Genre Group::calcBestGenre()
{
    std::shared_ptr<GenreStats> actual_stats = calcActualGroupStats();
    int comedy = actual_stats->getComedyStats(); //calcActualGroupViews = sum_users_views + multiplied
    int drama = actual_stats->getDramaStats();
    int action = actual_stats->getActionStats();
    int fantasy = actual_stats->getFantasyStats();

    if(comedy >= drama && comedy >= action && comedy >= fantasy)
    {
        return Genre::COMEDY;
    }

    if(drama >= action && drama >= comedy && drama >= fantasy)
    {
        return Genre::DRAMA;
    }

    if(action >= drama && action >= comedy && action >= fantasy)
    {
        return Genre::ACTION;
    }

    return Genre::FANTASY;
}

std::shared_ptr<Movie> Group::calcBestMovie(std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> comedy_tree,
                                       std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> drama_tree,
                                       std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> action_tree,
                                       std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> fantasy_tree)
{
    std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>> best_movie_node = nullptr;
    Genre top_genre = calcBestGenre();
    switch (top_genre)
    {
        case Genre::COMEDY:
            best_movie_node = comedy_tree->getMax();
            break;
        case Genre::DRAMA:
            best_movie_node = drama_tree->getMax();
            break;
        case Genre::ACTION:
            best_movie_node = action_tree->getMax();
            break;
        case Genre::FANTASY:
            best_movie_node = fantasy_tree->getMax();
            break;
        case Genre::NONE:
            break;
    }
    if(best_movie_node)
    {
        return best_movie_node->getValue();
    }
    return nullptr;
}

void Group::incSumUserViews(Genre genre)
{
    switch(genre)
    {
        case Genre::DRAMA:
            sum_users_views->incDramaStats();
            break;
        case Genre::ACTION:
            sum_users_views->incActionStats();
            break;
        case Genre::COMEDY:
            sum_users_views->incComedyStats();
            break;
        case Genre::FANTASY:
            sum_users_views->incFantasyStats();
            break;
        case Genre::NONE:
            break;
    }
}