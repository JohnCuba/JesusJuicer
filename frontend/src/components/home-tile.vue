<script lang="ts" setup></script>

<template>
  <div class="tile">
    <slot />
  </div>
</template>

<style lang="css" scoped>
.tile {
  position: relative;
  display: flex;
  flex-direction: column;
  justify-content: flex-end;
  aspect-ratio: 1;
  height: 10rem;
  width: 10rem;
  padding: 0.6rem;
  color: var(--palette-contrast);
  backdrop-filter: brightness(0.6);
  box-shadow: 0px 10px 15px -3px var(--palette-neutral);
  border-radius: 0.6rem;
  overflow: hidden;
}
</style>
