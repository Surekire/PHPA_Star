const ts = document.querySelector("#theme_selector");

const observer = new MutationObserver((mutations) => {
    for (const m of mutations) {
        if (m.type === "attributes" && m.attributeName === "data-clicked_value") {
            console.log("Set Theme to:", ts.dataset.clicked_value);
            document.querySelector("body").dataset.theme = ts.dataset.clicked_value;
        }
    }
});


observer.observe(ts, { attributes: true, attributeFilter: ["data-clicked_value"] });