import {Footer, Layout, Navbar} from 'nextra-theme-docs'
import {Head} from 'nextra/components'
import {getPageMap} from 'nextra/page-map'
import './globals.css'
import {Metadata} from "next";
import {NextraSearchDialog} from "@/components/nextra-search-dialog";
import {getPagesFromPageMap} from "@/lib/getPagesFromPageMap";

export const metadata: Metadata = {
    // Define your metadata here
    // For more information on metadata API, see: https://nextjs.org/docs/app/building-your-application/optimizing/metadata
}

const navbar = (
    <Navbar
        projectLink="https://github.com/alvinng4/c_traceback"
        logo={
            <div style={{ display: "flex", alignItems: "center", gap: "0.5rem" }}>
            <img
                src="/images/general/logo.svg"
                alt="Logo light"
                width={20}
                height={20}
                className="block dark:hidden"
            />
            <img
                src="/images/general/logo_dark.svg"
                alt="Logo dark"
                width={20}
                height={20}
                className="hidden dark:block"
            />
            <span style={{ fontWeight: "bold", fontSize: "1.1rem" }}>
                C Traceback
            </span>
            </div>
        }
        // ... Your additional navbar options
    />
)
const footer = <Footer>MIT {new Date().getFullYear()} © Ching-Yin Ng.</Footer>

export default async function RootLayout({children}) {
    const pageMap = await getPageMap();
    const pages = await getPagesFromPageMap({
        pageMapArray: pageMap,
        // modify page data if needed
        // filterItem: async (item) => {
        //     return {
        //         ...item,
        //     };
        // }
    });


    return (
        <html
            // Not required, but good for SEO
            lang="en"
            // Required to be set
            dir="ltr"
            // Suggested by `next-themes` package https://github.com/pacocoursey/next-themes#with-app
            suppressHydrationWarning
        >
        <Head
            // ... Your additional head options
        >
            {/* <link rel="shortcut icon" href="/images/general/icon.svg"/> */}
            {/* Your additional tags should be passed as `children` of `<Head>` element */}
        </Head>
        <body>
        <Layout
            navbar={navbar}
            pageMap={pageMap}
            docsRepositoryBase="https://github.com/alvinng4/c_traceback/tree/main"
            footer={footer}
            search={
                <div className="w-full md:w-36 lg:w-48"> 
                    <NextraSearchDialog pages={pages}/>
                </div>
            }
            // ... Your additional layout options
        >
            {children}
        </Layout>
        </body>
        </html>
    )
}