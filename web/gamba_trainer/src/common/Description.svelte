<script>
    export let title;
    export let explanation;
</script>

<div class="description">
    <h2>{title}</h2>
    <p>{explanation}</p>
</div>

<style lang="scss">
    @import "@scss/vars";

    .description {
        padding: 48px 32px;
        background-color: $color-lightsky;
        border-radius: 8px;
        margin-bottom: 64px;

        h2 {
            margin-bottom: 16px;
            font-size: 2.25rem;
        }
    }
</style>
