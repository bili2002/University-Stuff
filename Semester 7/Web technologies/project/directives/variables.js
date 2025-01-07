const audioElement = document.getElementById("audio-element");
const audioContext = new (window.AudioContext || window.webkitAudioContext)();
let audioSource = audioContext.createMediaElementSource(audioElement);

let currentFilter = null;
let analyserNode = audioContext.createAnalyser();
analyserNode.fftSize = 2048;
let bufferLength = analyserNode.frequencyBinCount;
let dataArray = new Uint8Array(bufferLength);

const canvasWaveform = document.getElementById("waveform");
const canvasWaveformCtx = canvasWaveform.getContext("2d");

const canvasFrequency = document.getElementById("frequency");
const canvasFrequencyCtx = canvasFrequency.getContext("2d");

const cutoffSlider = document.getElementById("cutoff-slider");
const cutoffValue = document.getElementById("cutoff-value");

const qSlider = document.getElementById("q-slider");
const qValue = document.getElementById("q-value");

const gainSlider = document.getElementById("gain-slider");
const gainValue = document.getElementById("gain-value");

const playPauseButton = document.getElementById("play-pause-button");
