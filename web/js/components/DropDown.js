export class DropDown extends HTMLElement {
    constructor() {
        super();
        
        const kids = Array.from(this.children);
        const buttonContent = kids.shift();
        const listContent = kids.shift();
        listContent.classList.add("list");
        
        this.innerHTML = `
            <div class="btn"></div>
        `;
        
        const btn = this.querySelector(".btn");
        btn.appendChild(buttonContent);
        this.appendChild(listContent);
        
        btn.addEventListener("click", (e) => {
            e.stopPropagation();

            // schließe alle anderen offenen Dropdowns
            document.querySelectorAll("dropdown-menu.open").forEach(d => {
                if (d !== this && !d.contains(this)) {
                d.classList.remove("open");
                }
            });

            this.classList.toggle("open");
        });


        const items = Array.from(listContent.children);
        items.forEach((item, index) => {
            if (!item.dataset.value && item.tagName !== "DROPDOWN-MENU") {
                item.dataset.value = item.textContent.trim();
            }

            item.dataset.index = index;

        });

        listContent.addEventListener("click", (e) => {
            e.stopPropagation();
            if (!e.target.matches("[data-value]")) return;

            const item = e.target;

            this.dataset.clicked_value = item.dataset.value;
            this.dataset.clicked_index = item.dataset.index;
        });
    }

}
