
function visualize() {
    function drawWaveform() {
        analyserNode.getByteTimeDomainData(dataArray);
        canvasWaveformCtx.fillStyle = "#f0f0f0";
        canvasWaveformCtx.fillRect(0, 0, canvasWaveform.width, canvasWaveform.height);
        canvasWaveformCtx.lineWidth = 2;
        canvasWaveformCtx.strokeStyle = "#007acc";
        canvasWaveformCtx.beginPath();

        const sliceWidth = canvasWaveform.width / bufferLength;
        let x = 0;

        for (let i = 0; i < bufferLength; i++) {
            const v = dataArray[i] / 128.0;
            const y = v * canvasWaveform.height / 2;

            if (i === 0) {
                canvasWaveformCtx.moveTo(x, y);
            } else {
                canvasWaveformCtx.lineTo(x, y);
            }

            x += sliceWidth;
        }
        canvasWaveformCtx.lineTo(canvasWaveform.width, canvasWaveform.height / 2);
        canvasWaveformCtx.stroke();
    }

    function drawFrequency() {
        analyserNode.getByteFrequencyData(dataArray);
        canvasFrequencyCtx.fillStyle = "#f0f0f0";
        canvasFrequencyCtx.fillRect(0, 0, canvasFrequency.width, canvasFrequency.height);
        canvasFrequencyCtx.fillStyle = "#ff5733";

        const barWidth = canvasFrequency.width / bufferLength * 2.5;
        let x = 0;

        for (let i = 0; i < bufferLength; i++) {
            const barHeight = dataArray[i] / 2;
            canvasFrequencyCtx.fillRect(x, canvasFrequency.height - barHeight, barWidth, barHeight);
            x += barWidth + 1;
        }
    }

    if (isPlaying) {
        drawWaveform();
        drawFrequency();
        requestAnimationFrame(visualize);
    }
}
