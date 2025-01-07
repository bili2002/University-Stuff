document.addEventListener("DOMContentLoaded", function() {
    var modal = document.getElementById("modalGallery");
    var modalImg = document.getElementById("modalImage");
    var captionText = document.getElementById("caption");
    var span = document.querySelector(".close-gallery");

    var images = document.querySelectorAll("img");

    images.forEach(function(image) {
        image.addEventListener('click', function() {
            modal.style.display = "block";
            modalImg.src = this.src;
            captionText.innerHTML = this.alt;
        });
    });

    span.addEventListener('click', function() {
        modal.style.display = "none";
    });

    modal.addEventListener('click', function(event) {
        if (event.target == modal) {
            modal.style.display = "none";
        }
    });
});
