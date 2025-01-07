
function createFilter(type, frequency = 1000, q = 1, gain = 0) {
    const filter = audioContext.createBiquadFilter();
    filter.type = type;
    filter.frequency.value = frequency;
    filter.Q.value = q;

    if (["peaking", "lowshelf", "highshelf"].includes(type)) {
        filter.gain.value = gain;
    }
    return filter;
}

function setFilter(filterType) {
    if (currentFilter) {
        audioSource.disconnect();
        currentFilter.disconnect();
    }

    const frequency = parseFloat(cutoffSlider.value); 
    const q = parseFloat(qSlider.value); 
    const gain = parseFloat(gainSlider.value); 

    currentFilter = createFilter(filterType, frequency, q, gain);
    audioSource.connect(currentFilter).connect(analyserNode).connect(audioContext.destination);
}

function updateFilterDetails(filterType) {
    const details = filterDetails[filterType];

    document.getElementById("filter-title").textContent = details.title;
    document.getElementById("filter-description").textContent = details.description;

    const howItWorksSection = document.getElementById("how-it-works");
    howItWorksSection.textContent = details.howItWorks;

    const applicationsList = document.getElementById("applications-list");
    applicationsList.innerHTML = "";
    details.applications.forEach(app => {
        const li = document.createElement("li");
        li.textContent = app;
        applicationsList.appendChild(li);
    });

    const characteristicsList = document.getElementById("characteristics-list");
    characteristicsList.innerHTML = "";
    details.characteristics.forEach(item => {
        const li = document.createElement("li");
        li.innerHTML = item;
        characteristicsList.appendChild(li);
    });

    const parametersList = document.getElementById("parameters-list");
    parametersList.innerHTML = "";
    details.parameters.forEach(item => {
        const li = document.createElement("li");
        li.innerHTML = item;
        parametersList.appendChild(li);
    });

    document.getElementById("filter-code").textContent = details.code.trim();
}


function updateUI(filterType) {
    const qControl = document.getElementById("q-control");
    if (["lowpass", "highpass", "bandpass", "notch", "allpass", "peaking"].includes(filterType)) {
        qControl.style.display = "flex";
    } else {
        qControl.style.display = "none";
    }

    const gainControl = document.getElementById("gain-control");
    if (["peaking", "lowshelf", "highshelf"].includes(filterType)) {
        gainControl.style.display = "flex";
    } else {
        gainControl.style.display = "none";
    }

    if (filterType === "lowpass" || filterType === "highpass") {
        cutoffSlider.value = 1000;
        qSlider.value = 1;
    } else if (filterType === "lowshelf" || filterType === "highshelf") {
        cutoffSlider.value = 200;
        gainSlider.value = 0;
    } else if (filterType === "peaking") {
        cutoffSlider.value = 1000;
        qSlider.value = 1;
        gainSlider.value = 5;
    }

    document.getElementById("cutoff-value").textContent = cutoffSlider.value;
    document.getElementById("q-value").textContent = qSlider.value;
    document.getElementById("gain-value").textContent = gainSlider.value;
}