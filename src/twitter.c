//      twitter.c
//      
//      Copyright 2011 Alfredo Liguoro <freddy@haxaddicts.net>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "core.h"

#include <oauth.h>

char *get_value(char **array,int len,const char *param_name)
{
	int i;
	for(i = 0;i < len;i++)
	{
		if(strncmp(array[i],param_name,strlen(param_name)) == 0)
			return array[i] + strcspn(array[i], "=") + 1;
	}
	return NULL;
}

char *ObtainToken(void)
{
	char *signed_url,*params,*url;
	char **split = NULL;
	int len;
	signed_url = oauth_sign_url2("http://api.twitter.com/oauth/request_token",NULL,OA_HMAC,NULL,CONSUMER_KEY,CONSUMER_SECRET,NULL,NULL);
	params = oauth_http_get(signed_url,NULL);
	free(signed_url);
	len = oauth_split_url_parameters(params,&split);
	asprintf(&url,"http://api.twitter.com/oauth/authorize?oauth_token=%s",get_value(split,len,"oauth_token"));
	gtk_show_uri(NULL,url,GDK_CURRENT_TIME,NULL);
	free(url);
	return params;
}

void ValidatePIN(const char *pin,const char *token,Twitter_user *user)
{
	char **split = NULL;
	char *dummy = NULL;
	int len;
	char *verify_url,*user_key,*verify_signed;
	len = oauth_split_url_parameters(token,&split);
	asprintf(&verify_url,"%s?oauth_verifier=%s","http://api.twitter.com/oauth/access_token",pin);
	verify_signed = oauth_sign_url2(verify_url,&dummy,OA_HMAC,NULL,CONSUMER_KEY,CONSUMER_SECRET,get_value(split,len,"oauth_token"),get_value(split,len,"oauth_token_secret"));
	user_key = oauth_http_post(verify_signed,dummy);
	free(verify_signed);
	free(verify_url);
	len = oauth_split_url_parameters(user_key,&split);
	strcpy(user->screen_name,get_value(split,len,"screen_name"));
	strcpy(user->token,get_value(split,len,"oauth_token"));
	strcpy(user->secret,get_value(split,len,"oauth_token_secret"));
	strcpy(user->id,get_value(split,len,"user_id"));
}

void Tweet(const char *msg,Twitter_user *user)
{
	char *tweet_url,*dummy = NULL,*signed_url;
	asprintf(&tweet_url,"%s%s","http://api.twitter.com/1/statuses/update.xml?status=",oauth_url_escape(msg));
	signed_url = oauth_sign_url2(tweet_url,&dummy,OA_HMAC,NULL,CONSUMER_KEY,CONSUMER_SECRET,user->token,user->secret);
	oauth_http_post(signed_url,dummy);
	free(signed_url);
	free(tweet_url);
}
