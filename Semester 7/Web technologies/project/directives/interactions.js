let isPlaying = false;
playPauseButton.addEventListener("click", () => {
    if (!isPlaying) {
        audioContext.resume();
        audioElement.play();
        playPauseButton.textContent = "Спри";
        isPlaying = true;
        visualize();
    } else {
        audioElement.pause();
        playPauseButton.textContent = "Пусни";
        isPlaying = false;
    }
});

cutoffSlider.addEventListener("input", event => {
    if (currentFilter) {
        const value = event.target.value;
        currentFilter.frequency.value = value;
        cutoffValue.textContent = value;
    }
});

qSlider.addEventListener("input", event => {
    if (currentFilter) {
        const value = event.target.value;
        currentFilter.Q.value = value;
        qValue.textContent = value;
    }
});

gainSlider.addEventListener("input", event => {
    if (currentFilter && ["peaking", "lowshelf", "highshelf"].includes(currentFilter.type)) {
        const gain = parseFloat(event.target.value);
        currentFilter.gain.value = gain;
        gainValue.textContent = gain.toFixed(1);
    }
});

document.querySelectorAll(".filter-button").forEach(button => {
    button.addEventListener("click", event => {
        const filterType = event.target.dataset.filter;
        updateFilterDetails(filterType);
        updateUI(filterType);
        setFilter(filterType);
    });
});

document.addEventListener("DOMContentLoaded", () => {
    const defaultFilter = "lowpass";
    updateFilterDetails(defaultFilter);
    updateUI(defaultFilter);
    setFilter(defaultFilter);
});

