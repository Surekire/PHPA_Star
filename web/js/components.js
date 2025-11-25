import { DropDown } from "./components/DropDown.js";
customElements.define('dropdown-menu', DropDown);

document.addEventListener("click", (e) => {
    const path = e.composedPath();
    console.log("test");
    document.querySelectorAll("dropdown-menu.open").forEach(d => {
        if (!path.includes(d)) {
            d.classList.remove("open");
        }
    });
});



import * as SideElements from "./components/SideElements.js";
customElements.define('header', SideElements.Header);
customElements.define('footer', SideElements.Footer);
