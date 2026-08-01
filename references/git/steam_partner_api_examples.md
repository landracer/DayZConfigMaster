# Overview
* User API key- https://steamcommunity.com/dev/apikey
    * Some APIs will require a game publisher API key instead of a user API key
* Official Documentation - https://partner.steamgames.com/doc/webapi
* Non-official documentation - https://steamapi.xpaw.me/
    * This contains a lot of secret but usable API endpoints


# Contents
* [Get information about a workshop addon](#get-workshop-item-details)
* [Get information about a workshop collection](#get-workshop-collection-details)
* [Get information about a user's workshop files](#get-a-users-workshop-files)


<br><br><br>



# Get workshop item details
This retrieves information about an addon on the workshop. <br>
Full details available through the [unofficial documentation](https://steamapi.xpaw.me/#IPublishedFileService)

## Request
* `GET` https://api.steampowered.com/IPublishedFileService/GetUserFiles/v1/
* Parameters are provided using: **query parameters** <br>
* A [User API key](https://steamcommunity.com/dev/apikey) is required.

<br>

### Example
Get information about https://steamcommunity.com/sharedfiles/filedetails/?id=2086515808

`https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=USER_API_KEY_HERE&includevotes=true&publishedfileids[0]=2086515808`

* `key` USER_API_KEY_HERE
* `includevotes` true
* `publishedfileids[0]` 2086515808

<br>

### Example
Get information about multiple items

`https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=USER_API_KEY_HERE&includevotes=true&publishedfileids[0]=2086515808&publishedfileids[1]=2075704025&publishedfileids[2]=2063612379`

* `key` USER_API_KEY_HERE
* `includevotes` true
* `publishedfileids[0]` 2086515808
* `publishedfileids[1]` 2075704025
* `publishedfileids[2]` 2063612379

<br>

## Example response
```json
{
    "response": {
        "publishedfiledetails": [
            {
                "result": 1,
                "publishedfileid": "2086515808",
                "creator": "76561198021181972",
                "creator_appid": 4000,
                "consumer_appid": 4000,
                "consumer_shortcutid": 0,
                "filename": "",
                "file_size": "54663864",
                "preview_file_size": "94432",
                "preview_url": "https://steamuserimages-a.akamaihd.net/ugc/1027328386892894710/6053E3731EBD29024BF2E5B3155B7EA028FE1E3E/",
                "url": "",
                "hcontent_file": "8378447467192818806",
                "hcontent_preview": "1027328386892894710",
                "title": "TTT Windmill in the sky",
                "file_description": "Fragments of land have drifted together to form a new tranquil land, floating way above the cities.\r\nWhile the gardens are peaceful, a slip may result in a soul being lost to the clouds.\r\n\r\n[h1]Feedback is greatly appreciated! No matter how small.[/h1]\r\n\r\n\r\n[h1]Overview[/h1]\r\nThis map was made for the [url=https://www.fluffyservers.com/mapathon.html]GMod Mapping Community Mapathon[/url] \r\n(April 17th - May 4th)\r\n\r\nIt is suitable for around 20 players.\r\nThe map features several islands in the sky. Falling off will result in death.\r\n\r\n[b]Map Name[/b]: ttt_windmill_sky\r\n\r\n\r\n[h1]Updates[/h1]\r\nAs this map was made for a competition, I plan to release several updates sometime after the submission date (May 4th 2020).\r\n[b]Subscribe[/b] and [b]favourite[/b] to receive the latest updates!\r\n\r\n\r\nIf you like this addon, please make sure to [b]Like[/b] it to help its ratings.",
                "time_created": 1588735858,
                "time_updated": 1588735858,
                "visibility": 0,
                "flags": 5632,
                "workshop_file": false,
                "workshop_accepted": false,
                "show_subscribe_all": false,
                "num_comments_developer": 0,
                "num_comments_public": 7,
                "banned": false,
                "ban_reason": "",
                "banner": "76561197960265728",
                "can_be_deleted": true,
                "app_name": "Garry's Mod",
                "file_type": 0,
                "can_subscribe": true,
                "subscriptions": 477,
                "favorited": 16,
                "followers": 0,
                "lifetime_subscriptions": 530,
                "lifetime_favorited": 16,
                "lifetime_followers": 0,
                "lifetime_playtime": "0",
                "lifetime_playtime_sessions": "0",
                "views": 300,
                "num_children": 0,
                "num_reports": 0,
                "tags": [
                    {
                        "tag": "Addon",
                        "adminonly": false
                    },
                    {
                        "tag": "map",
                        "adminonly": false
                    },
                    {
                        "tag": "Scenic",
                        "adminonly": false
                    }
                ],
                "vote_data": {
                    "score": 0.555555582046508789,
                    "votes_up": 15,
                    "votes_down": 2
                },
                "language": 0,
                "maybe_inappropriate_sex": false,
                "maybe_inappropriate_violence": false,
                "revision_change_number": "10",
                "revision": 1
            }
        ]
    }
}
```








<br><br><br>









# Get workshop collection details
This retrieves information about a collection on the workshop. <br>
Full details available through the [Steam Documentation](https://partner.steamgames.com/doc/webapi/ISteamRemoteStorage#EnumerateUserSubscribedFiles)

## Request
* `GET` https://api.steampowered.com/IPublishedFileService/GetUserFiles/v1/
* Parameters are provided using: **query parameters** <br>
* A [User API key](https://steamcommunity.com/dev/apikey) is required.

<br>

### Example
Get information about https://steamcommunity.com/sharedfiles/filedetails/?id=2081627614

`https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=USER_API_KEY_HERE&includevotes=true&includechildren=true&publishedfileids[0]=2081627614`

* `key` USER_API_KEY_HERE
* `includevotes` true
* `includechildren` true
* `publishedfileids[0]` 2081627614

<br>

### Example
Get information about multiple collections
`https://api.steampowered.com/IPublishedFileService/GetDetails/v1/?key=USER_API_KEY_HERE&includevotes=true&includechildren=true&publishedfileids[0]=2081627614&publishedfileids[1]=2045241070&publishedfileids[2]=2045248309`

* `key` USER_API_KEY_HERE
* `includevotes` true
* `includechildren` true
* `publishedfileids[0]` 2081627614
* `publishedfileids[1]` 2045241070
* `publishedfileids[2]` 2045248309

<br>

## Example response
```json
{
    "response": {
        "publishedfiledetails": [
            {
                "result": 1,
                "publishedfileid": "2081627614",
                "creator": "76561198021181972",
                "creator_appid": 766,
                "consumer_appid": 4000,
                "consumer_shortcutid": 0,
                "filename": "2081627614_preview_workshop-map-thumbnail.jpg",
                "file_size": "45165",
                "preview_file_size": "45165",
                "file_url": "https://steamuserimages-a.akamaihd.net/ugc/1021698451631735351/5C93D037BAD53BF014F41D3148831FB1D4480D16/",
                "preview_url": "https://steamuserimages-a.akamaihd.net/ugc/1021698451631735351/5C93D037BAD53BF014F41D3148831FB1D4480D16/",
                "url": "",
                "hcontent_file": "1021698451631735351",
                "hcontent_preview": "1021698451631735351",
                "title": "Garry's Mod Mapping Community Mapathon 2020",
                "file_description": "Maps created for the 2020 [url=https://discord.gg/9qjPWQg]Garry's Mod Mapping Community[/url] Mapathon.\r\nApril 17th - May 4th\r\n\r\n\r\n[h1]Theme[/h1]\r\n[quote]In this Mapathon, the theme is Verticality - this means you should make a map which uses verticality in a creative manner. We'll be flexible when judging - use your imagination, so long as the map isn't flat! Anything from a skyscraper to a sprawling stacked city could make an exciting map.[/quote]\r\n\r\n\r\n\r\n[h1]Private Maps[/h1]\r\n[url=https://steamcommunity.com/sharedfiles/filedetails/?id=2084595885][img]https://i.imgur.com/OQaLjdY.png[/img][/url]",
                "time_created": 1588415498,
                "time_updated": 1588784931,
                "visibility": 0,
                "flags": 1536,
                "workshop_file": false,
                "workshop_accepted": false,
                "show_subscribe_all": false,
                "num_comments_developer": 0,
                "num_comments_public": 1,
                "banned": false,
                "ban_reason": "",
                "banner": "76561197960265728",
                "can_be_deleted": true,
                "app_name": "Garry's Mod",
                "file_type": 2,
                "can_subscribe": false,
                "subscriptions": 0,
                "favorited": 4,
                "followers": 0,
                "lifetime_subscriptions": 0,
                "lifetime_favorited": 4,
                "lifetime_followers": 0,
                "lifetime_playtime": "0",
                "lifetime_playtime_sessions": "0",
                "views": 166,
                "image_width": 512,
                "image_height": 512,
                "num_children": 18,
                "num_reports": 0,
                "tags": [
                    {
                        "tag": "map",
                        "adminonly": false
                    },
                    ...
                ],
                "children": [
                    {
                        "publishedfileid": "2075704025",
                        "sortorder": 1,
                        "file_type": 0
                    },
                    {
                        "publishedfileid": "2063612379",
                        "sortorder": 2,
                        "file_type": 0
                    },
                    {
                        "publishedfileid": "2080687161",
                        "sortorder": 3,
                        "file_type": 0
                    },
                    ...
                ],
                "vote_data": {
                    "score": 0.545454561710357666,
                    "votes_up": 10,
                    "votes_down": 0
                },
                "language": 0,
                "maybe_inappropriate_sex": false,
                "maybe_inappropriate_violence": false,
                "revision_change_number": "13",
                "revision": 1
            }
        ]
    }
}
```








<br><br><br>









# Get a user's workshop files
This retrieves information about a user's items on the workshop. <br>
Full details available through the [unofficial documentation](https://steamapi.xpaw.me/#IPublishedFileService/GetUserFiles)

## Request
* `GET` https://api.steampowered.com/IPublishedFileService/GetUserFiles/v1/
* Parameters are provided using: **query parameters** <br>
* A [User API key](https://steamcommunity.com/dev/apikey) is required.

<br>

### Example
Get workshop files for user http://steamcommunity.com/profiles/76561198021181972

`https://api.steampowered.com/IPublishedFileService/GetUserFiles/v1/?key=USER_API_KEY_HERE&steamid=76561198021181972`

* `key` USER_API_KEY_HERE
* `steamid` 76561198021181972

<br>

### Example
Get the first 5 Garry's Mod workshop items for user http://steamcommunity.com/profiles/76561198021181972

`https://api.steampowered.com/IPublishedFileService/GetUserFiles/v1/?key=USER_API_KEY_HERE&steamid=76561198021181972&appid=4000&numperpage=5&page=1`

* `key` USER_API_KEY_HERE
* `steamid` 76561198021181972
* `appid` 4000
* `numperpage` 5
* `page` 1

<br>

## Example Response
```json
{
    "response": {
        "total": 15,
        "startindex": 1,
        "publishedfiledetails": [
            {
                "result": 1,
                "publishedfileid": "2086515808",
                "creator": "76561198021181972",
                "creator_appid": 4000,
                "consumer_appid": 4000,
                "consumer_shortcutid": 0,
                "filename": "",
                "file_size": "54663864",
                "preview_file_size": "94432",
                "preview_url": "https://steamuserimages-a.akamaihd.net/ugc/1027328386892894710/6053E3731EBD29024BF2E5B3155B7EA028FE1E3E/",
                "url": "",
                "hcontent_file": "8378447467192818806",
                "hcontent_preview": "1027328386892894710",
                "title": "TTT Windmill in the sky",
                "short_description": "Fragments of land have drifted together to form a new tranquil land, floating way above the cities.\r\nWhile the gardens are peaceful, a slip may result in a soul being lost to the clouds.\r\n\r\n[h1]Feedback is greatly appreciated! No matter how small.[/h1]\r\n\r\n",
                "time_created": 1588735858,
                "time_updated": 1588735858,
                "visibility": 0,
                "flags": 5632,
                "workshop_file": false,
                "workshop_accepted": false,
                "show_subscribe_all": false,
                "num_comments_developer": 0,
                "num_comments_public": 7,
                "banned": false,
                "ban_reason": "",
                "banner": "76561197960265728",
                "can_be_deleted": true,
                "app_name": "Garry's Mod",
                "file_type": 0,
                "can_subscribe": true,
                "subscriptions": 469,
                "favorited": 15,
                "followers": 0,
                "lifetime_subscriptions": 520,
                "lifetime_favorited": 15,
                "lifetime_followers": 0,
                "lifetime_playtime": "0",
                "lifetime_playtime_sessions": "0",
                "views": 291,
                "num_children": 0,
                "num_reports": 0,
                "tags": [
                    {
                        "tag": "Addon",
                        "adminonly": false
                    },
                    ...
                ],
                "vote_data": {
                    "score": 0.555555582046508789,
                    "votes_up": 15,
                    "votes_down": 2
                },
                "language": 0,
                "maybe_inappropriate_sex": false,
                "maybe_inappropriate_violence": false,
                "revision_change_number": "10",
                "revision": 1
            },
            ...
        ],
        "apps": [
            {
                "appid": 4000,
                "name": "Garry's Mod",
                "private": false
            }
        ]
    }
}
```