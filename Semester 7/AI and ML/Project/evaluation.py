import pandas as pd
import numpy as np
import random

import dataframes

def evaluate_recommendator(
    recommender,
    df=dataframes.df_user_data_cleaned,
    sample_users=100,
    k=10,
    test_games_perc=0.5,
    draws_per_user=3
):
    user_to_games = df.groupby("user_id")["appid"].apply(list).to_dict()
    
    all_users = list(user_to_games.keys())
    if sample_users is not None and sample_users < len(all_users):
        eval_users = random.sample(all_users, sample_users)
    else:
        eval_users = all_users

    precision_list = []
    recall_list = []
    f1_list = []

    for user_id in eval_users:
        games_list = user_to_games[user_id]
        
        user_precisions = []
        user_recalls = []
        
        test_games = int(len(games_list) * test_games_perc)
        if test_games == 0 or test_games == len(games_list):
            continue

        for _ in range(draws_per_user):
            test_sample = random.sample(games_list, test_games)
            train_games = list(set(games_list) - set(test_sample))
            
            recommended = recommender(train_games, k)

            overlap = set(test_sample).intersection(set(recommended[:k]))
            overlap_count = len(overlap)

            precision = overlap_count / k
            user_precisions.append(precision)

            recall = overlap_count / test_games
            user_recalls.append(recall)
        
        avg_p = np.mean(user_precisions)
        avg_r = np.mean(user_recalls)
        avg_f1 = 2 * (avg_p * avg_r) / (avg_p + avg_r) if (avg_p + avg_r) > 0 else 0
        
        precision_list.append(avg_p)
        recall_list.append(avg_r)
        f1_list.append(avg_f1)

    mean_precision = np.mean(precision_list)
    mean_recall = np.mean(recall_list)
    mean_f1 = np.mean(f1_list)
    
    return {
        "precision_at_k": mean_precision,
        "recall_at_k": mean_recall,
        "f1_at_k": mean_f1
    }
