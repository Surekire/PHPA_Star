import { DropDown } from "./components/DropDown.js";
customElements.define('dropdown-menu', DropDown);

document.addEventListener("click", (e) => {
    const path = e.composedPath();
    document.querySelectorAll("dropdown-menu.open").forEach(d => {
        if (!path.includes(d)) {
            d.classList.remove("open");
        }
    });
});



import * as SideElements from "./components/SideElements.js";
customElements.define('header-bar', SideElements.Header);
customElements.define('footer-bar', SideElements.Footer);
customElements.define('side-pannel', SideElements.SidePannel);
