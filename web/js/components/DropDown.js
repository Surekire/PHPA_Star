export class DropDown extends HTMLElement {
    constructor() {
        super();
        const shadow = this.attachShadow({ mode: 'open' });
        shadow.innerHTML = `
            <div class="btn"></div>
        `;

        const btn = shadow.querySelector(".btn");

        const kids = Array.from(this.children);
        const buttonContent = kids.shift();
        const listContent = kids.shift();
        listContent.classList.add("list");

        btn.appendChild(buttonContent);
        shadow.appendChild(listContent);

        btn.addEventListener("click", (e) => {
            e.stopPropagation();
            this.classList.toggle("open");
        });

        const items = Array.from(listContent.children);
        items.forEach((item, index) => {
            if (!item.dataset.value) {
                item.dataset.value = item.textContent.trim();
            }

            item.dataset.index = index;
        });

        listContent.addEventListener("click", (e) => {
            if (!e.target.matches("[data-value]")) return;

            const item = e.target;

            this.dataset.clicked_value = item.dataset.value;
            this.dataset.clicked_index = item.dataset.index;
        });
    }

    connectedCallback() {
        // Klick außerhalb schließt nur *dieses* Dropdown
        document.addEventListener("click", (e) => {
            if (!this.contains(e.target)) {
                this.classList.remove("open");
            }
        });
    }

}
