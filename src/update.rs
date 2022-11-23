use exitfailure::ExitFailure;
use reqwest::Url;
use serde_derive::{Deserialize, Serialize};
use std::env;

// #[derive(Serialize, Deserialize, Debug)]
// struct CommandQuote {
//     v: f64,

// }

// impl CommandQuote {
//     async fn get(symbol: &String) -> Result<Self, ExitFailure> {
//         let url = format!("https://api.github.com/repos/twert627/xbrew");
//         let url = Url::parse(&*url)?;
//         let res = reqwest::get(url).await?.json::<CommandQuote>().await?;
//     }
// }


pub fn fetch_xbrew_version() {

}

pub fn update() {
    println!("{}", "Updating xbrew to version");
}
