var coll = document.getElementsByClassName("dev_console_button");
var i;

// For each dev_console_button instance
for (i = 0; i < coll.length; i++) {
  // Add event listener
  coll[i].style.borderRadius = "2px 2px 2px 2px";
  coll[i].addEventListener("click", function() {
    // this referres to coll[i]
    this.classList.toggle("active");
    // Get the next item in the html below button
    var content = this.nextElementSibling;
    if (content.style.maxHeight){
      content.style.maxHeight = null;
      this.style.borderRadius = "2px 2px 2px 2px";
    } else {
      content.style.maxHeight = content.scrollHeight + "px";
      this.style.borderRadius = "2px 2px 0px 0px";
    }
  });
}