import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans, BisectingKMeans
from sklearn.metrics import silhouette_score
import seaborn as sns
import sys

def plot_data_and_centroids(X, labels, centroids):
    plt.figure(figsize=(8, 6))
    sns.scatterplot(x=X[:, 0], y=X[:, 1], hue=labels, palette='Set1', s=100, legend='full')

    plt.scatter(centroids[:, 0], centroids[:, 1], c='black', s=300, marker='X', label='Centroids')
    
    plt.title('Data and Centroids Visualization')
    plt.legend()
    plt.show()


def evaluate_clustering(kmeans, data, labels, metric):
    if metric == "wcss":
        return kmeans.inertia_
    else:
        return silhouette_score(data, labels)


def run_kmeans(data, algorithm, init, metric, n_clusters):
    if algorithm == "kmeans":
        AlgorithmInit = KMeans
    elif algorithm == "bisecting":
        AlgorithmInit = BisectingKMeans

    if init == "random":
        best_metric_value = float('inf') if metric == "wcss" else float('-inf')
        best_labels = None
        best_centroids = None

        for _ in range(10):
            kmeans = AlgorithmInit(n_clusters=n_clusters, init="random", n_init=1)
            kmeans.fit(data)

            labels = kmeans.labels_
            centroids = kmeans.cluster_centers_
            metric_value = evaluate_clustering(kmeans, data, labels, metric)

            if (metric == "wcss" and metric_value < best_metric_value) or (metric != "wcss" and metric_value > best_metric_value):
                best_metric_value = metric_value
                best_labels = labels
                best_centroids = centroids
    else: # k-means++
        kmeans = KMeans(n_clusters=n_clusters, init="k-means++", n_init=10)
        kmeans.fit(data)

        best_labels = kmeans.labels_
        best_centroids = kmeans.cluster_centers_
        best_metric_value = evaluate_clustering(kmeans, data, best_labels, metric)

    print(f"{algorithm} algorithm with {metric} metric and {init} init: {best_metric_value}")
    plot_data_and_centroids(data, best_labels, best_centroids)


if __name__ == "__main__":
    file_path = sys.argv[1]
    data = np.loadtxt(file_path)[:, :2]

    algorithm = sys.argv[2]  # "kmeans" or "bisecting"
    init = sys.argv[3]  # "random" or "kmeans++"
    metric = sys.argv[4]  # "wcss" or "silhouette"
    n_clusters = int(sys.argv[5])

    run_kmeans(data, algorithm, init, metric, n_clusters)
