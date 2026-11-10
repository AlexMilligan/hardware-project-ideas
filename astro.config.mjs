// @ts-check
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';

// https://astro.build/config
export default defineConfig({
	site: 'https://alexmilligan.github.io',
	base: '/hardware-project-ideas',
	integrations: [
		starlight({
			title: 'Hardware Project Ideas',
			description:
				'Beginner electronics projects. A page is only as proven as its evidence line says.',
			social: [
				{
					icon: 'github',
					label: 'GitHub',
					href: 'https://github.com/AlexMilligan/hardware-project-ideas',
				},
			],
			sidebar: [
				{ label: 'Start here', slug: 'index' },
				{
					label: 'Projects',
					items: [
						{ label: 'All projects', slug: 'projects' },
						{ slug: 'projects/template' },
						{
							label: 'Binaural recorder',
							items: [
								{ label: 'Overview', slug: 'projects/binaural-recorder' },
								{
									label: 'Design and wiring',
									slug: 'projects/binaural-recorder/design-and-wiring',
								},
							],
						},
						{ slug: 'projects/social-battery-patch' },
					],
				},
			],
		}),
	],
});
