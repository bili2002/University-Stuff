import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

def initialize_centroids(data, k, init_method="random"):
    if init_method == "random":
        indices = np.random.choice(data.shape[0], k, replace=False)
        return data[indices]
    elif init_method == "kmeans++":
        centroids = [data[np.random.choice(data.shape[0])]]
        for _ in range(1, k):
            distances = np.min([np.linalg.norm(data - c, axis=1)**2 for c in centroids], axis=0)
            probabilities = distances / np.sum(distances)
            centroid = data[np.random.choice(data.shape[0], p=probabilities)]
            centroids.append(centroid)
        return np.array(centroids)

def assign_clusters(data, centroids):
    distances = np.linalg.norm(data[:, np.newaxis] - centroids, axis=2)
    return np.argmin(distances, axis=1)

def update_centroids(data, labels, k):
    return np.array([data[labels == i].mean(axis=0) for i in range(k)])

def compute_wcss(data, labels, centroids):
    return sum(np.sum((data[labels == i] - centroids[i])**2) for i in range(centroids.shape[0]))

def silhouette_score(data, labels, centroids):
    silhouette_values = []
    for i, point in enumerate(data):
        cluster = labels[i]
        same_cluster = data[labels == cluster]
        other_clusters = [data[labels == c] for c in range(centroids.shape[0]) if c != cluster]

        a = np.mean(np.linalg.norm(same_cluster - point, axis=1)) if len(same_cluster) > 1 else 0
        b = min(np.mean(np.linalg.norm(cluster - point, axis=1)) for cluster in other_clusters)

        silhouette_values.append((b - a) / max(a, b))
    return np.mean(silhouette_values)

def kmeans(data, k, init_method="random", max_iters=300, tol=1e-4):
    centroids = initialize_centroids(data, k, init_method)
    prev_centroids = centroids + tol + 1
    iter_count = 0

    while np.linalg.norm(centroids - prev_centroids) > tol and iter_count < max_iters:
        labels = assign_clusters(data, centroids)
        prev_centroids = centroids
        centroids = update_centroids(data, labels, k)
        iter_count += 1

    return labels, centroids

def plot_data_and_centroids(data, labels, centroids):
    plt.figure(figsize=(8, 6))
    sns.scatterplot(x=data[:, 0], y=data[:, 1], hue=labels, palette='Set1', s=100, legend='full')

    plt.scatter(centroids[:, 0], centroids[:, 1], c='black', s=300, marker='X', label='Centroids')

    plt.title('Data and Centroids Visualization')
    plt.legend()
    plt.show()

if __name__ == "__main__":
    import sys
    file_path = sys.argv[1]
    algorithm = sys.argv[2]  # "kmeans"
    init = sys.argv[3]  # "random" or "kmeans++"
    metric = sys.argv[4]  # "wcss" or "silhouette"
    n_clusters = int(sys.argv[5])

    data = np.loadtxt(file_path)[:, :2]
    labels, centroids = kmeans(data, n_clusters, init_method=init)

    if metric == "wcss":
        metric_value = compute_wcss(data, labels, centroids)
    elif metric == "silhouette":
        metric_value = silhouette_score(data, labels, centroids)

    print(f"{algorithm} algorithm with {metric} metric and {init} init: {metric_value}")
    plot_data_and_centroids(data, labels, centroids)