<script setup lang="ts">
const MENU = [{ name: 'home' }, { name: 'wi-fi' }]
</script>

<template>
  <header class="header">
    <h1 class="header__title">Aquaphobic</h1>
    <nav>
      <ul class="header__navigation">
        <li class="header__navigation-item" v-for="item in MENU" :key="item.name">
          <router-link :to="item">{{ item.name }}</router-link>
        </li>
      </ul>
    </nav>
  </header>
</template>

<style scoped>
.header {
  height: 22rem;
  display: flex;
  flex-direction: column;
  justify-content: flex-end;
  background-color: var(--palette-main);
}

.header__title {
  padding-inline: 1rem;
  font-size: 2.625rem;
  font-weight: 100;
  margin-bottom: 1rem;
}

.header__navigation {
  list-style: none;
  overflow: auto;
  display: flex;
  border-top: 0.1px solid var(--palette-contrast);
}

.header__navigation-item > a {
  font-size: 1.2em;
  padding: 0.8rem;
  color: var(--palette-contrast);
  display: block;
  border: none;
  text-align: start;
  text-transform: capitalize;
  text-decoration: none;
  cursor: pointer;
  position: relative;
}
</style>
