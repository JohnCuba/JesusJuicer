<script setup lang="ts">
import { computed } from 'vue'
import { useRoute } from 'vue-router'

defineProps({
  title: {
    type: String,
    default: 'Aquaphobic',
  },
  subtitle: {
    type: String,
    default: null,
  },
  withNav: {
    type: Boolean,
    default: false,
  },
})

const MENU_ITEMS = [{ name: 'home' }, { name: 'wi-fi' }]
const route = useRoute()

const menu = computed(() => MENU_ITEMS.filter(({ name }) => name !== route.name))
</script>

<template>
  <header class="header">
    <div v-if="$slots.default" class="header__content">
      <slot />
    </div>
    <h2 v-if="subtitle" class="header__subtitle">{{ subtitle }}</h2>
    <h1 class="header__title">{{ title }}</h1>
    <nav v-if="withNav">
      <ul class="header__navigation">
        <li class="header__navigation-item" v-for="item in menu" :key="item.name">
          <router-link :to="item">{{ item.name }}</router-link>
        </li>
      </ul>
    </nav>
  </header>
</template>

<style scoped>
.header {
  min-height: 22rem;
  display: flex;
  flex-direction: column;
  justify-content: flex-end;
  background-color: var(--palette-main);
}

.header__content {
  padding: 1rem;
  flex: 1;
}

.header__title {
  padding-inline: 1rem;
  font-size: 2.625rem;
  font-weight: 400;
  margin-bottom: 1rem;
}

.header__subtitle {
  padding-inline: 1rem;
  font-size: 1.625rem;
  font-weight: 100;
  margin-bottom: 0.4rem;
  opacity: 0.6;
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
