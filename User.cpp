//
// Created by layan on 5/19/2023.
//
#include "UserAndGroup.h"

std::shared_ptr<GenreStats> User::getUserGenreViews()
{
    return user_genre_views;
}

void User::updateUserGenreViews(std::shared_ptr<GenreStats> group_genre_views, bool sign)
{
    user_genre_views->mergeGenreStats(group_genre_views, sign);
}

/*std::shared_ptr<Group> User::getUserGroup()
{
    return user_group;
}*/

std::weak_ptr<Group> User::getUserGroup()
{
    return user_group;
}

/*void User::setUserGroup(std::shared_ptr<Group> new_group)
{
    //should free old pointer or something? ..
    if(new_group == nullptr)
    {
        user_group = nullptr;
        return;
    }
    user_group = new_group;
}*/

void User::setUserGroup(std::weak_ptr<Group> new_group)
{
    //should free old pointer or something? ..
    user_group = new_group;
}

bool User::isVip()
{
    return vip;
}

//returns the field inGroup
bool User::isInGroup()
{
    return inGroup;
}

//changes the field inGroup
void User::setInGroup(bool in_group)

{
    inGroup = in_group;
}

std::shared_ptr<GenreStats> User::calcActualUserStats()
{
    //if(!user_group) return user_genre_views;
    if(!inGroup) return user_genre_views;
    std::shared_ptr<GenreStats> actual_stats = std::make_shared<GenreStats>();
    actual_stats->mergeGenreStats(user_genre_views, true);
    actual_stats->mergeGenreStats(user_group.lock()->getExclusiveGroupViews(), true);
    return actual_stats;
}

std::shared_ptr<GenreStats> User::getExclusiveWhenJoined()
{
    return group_exclusive_views_when_joined;
}

void User::incUserGenreViews(Genre genre)
{
    switch(genre)
    {
        case Genre::DRAMA:
            user_genre_views->incDramaStats();
            break;
        case Genre::ACTION:
            user_genre_views->incActionStats();
            break;
        case Genre::COMEDY:
            user_genre_views->incComedyStats();
            break;
        case Genre::FANTASY:
            user_genre_views->incFantasyStats();
            break;
        case Genre::NONE:
            break;
    }
}
